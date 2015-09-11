#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
syntax::syntax( char *sourcename ) : table( sourcename ) {
	statstarters	=	setofsymbols( 6, ident, beginsy, readsy, writesy, ifsy, whilesy	);
	factorstarters	=	setofsymbols( 5, ident, intconst, charconst, notop, leftparent	);
	mulops			=	setofsymbols( 3, times, divop, andop							);
	signs			=	setofsymbols( 2, plus, minus									);
	addops			=	setofsymbols( 3, plus, minus, orop								);
	relops			=	setofsymbols( 6, eqop, neop, ltop, leop, geop, gtop				);
} // syntax( )

syntax::~syntax( ) {
	// empty
} // ~syntax( )

//////////////////////////
//// Public Functions ////
//////////////////////////
void syntax::programme( ) {
	...
} // programme( )

///////////////////////////
//// Private Functions ////
///////////////////////////
/**/bool syntax::compatible( typentry type1, typentry type2 ) {
/**/	// Decides whether types pointed at by type1 and type2 are compatible or not
/**/
/**/	if ( type1 == type2 ) {
/**/		return true;
/**/	} else if ( ( type1 == nil ) || ( type2 == nil ) ) {
/**/		return true;
/**/	} else if ( ( type1->form == arrays ) && ( type2->form = arrays ) ) {
/**/		return ( type1->indexmin == type2->indexmin ) &&
/**/			   ( type1->indexmax == type2->indexmax ) &&
/**/			   compatible( type1->elementtype, type2->elementtype );
/**/	} else { return false; }
/**/}

void syntax::syntaxerror( symboltype expectedsymbol ) {
...
} // syntaxerror( )

void syntax::accept( symboltype symbolexpected ) {
...
} // accept( )

void syntax::skipto( setofsymbols relevantsymbols ) {
...}

void syntax::startcheck( setofsymbols starters, setofsymbols followers ) {
...}


void syntax::endcheck( setofsymbols followers ) {
...}



void syntax::block( setofsymbols followers ) {
...
} // block( )

void syntax::statpart( setofsymbols followers ) {
...
} // statpart( )

void syntax::compoundstatement( setofsymbols followers ) {
...
} // compoundstatement( );

void syntax::statement( setofsymbols followers ) {
...
} // statement( )

void syntax::ifstatement( setofsymbols followers ) {
/**/typentry	exptype;

	accept( ifsy );
	expression( followers + thensy + elsesy/**/, exptype/**/ );
/**/if (! compatible( exptype, booltype ) ) {
/**/	semanticerror( 69 );
/**/}
	accept( thensy );
	statement( followers + elsesy );
	if ( symbol == elsesy ) {
		accept( elsesy );
		statement( followers );
	}
} // ifstatement( )

void syntax::whilestatement( setofsymbols followers ) {
/**/typentry	exptype;

	accept( whilesy );
	expression( followers + dosy/**/, exptype/**/ );
/**/if (! compatible( exptype, booltype ) ) {
/**/	semanticerror( 69 );
/**/}
	accept( dosy );
	statement( followers );
} // whilestatement( )

void syntax::outputvalue( setofsymbols followers ) {
/**/typentry	exptype;

	expression( followers + comma + rightparent/**/, exptype/**/ );
/**/if ( !( compatible( exptype, chartype ) ) &&
/**/	 !( compatible( exptype, inttype ) ) ) {
/**/	semanticerror( 68 );
/**/}
} // outputvalue( )

void syntax::writestatement( setofsymbols followers ) {
	accept( writesy );
	accept( leftparent );
	outputvalue( followers );
	while ( symbol == comma ) {
		accept( comma );
		outputvalue( followers );
	}
	accept( rightparent );
} // writestatement( )

void syntax::inputvariable( setofsymbols followers ) {
/**/typentry	vartype;

	variable( followers + comma + rightparent/**/, vartype/**/ );
/**/if ( !( compatible( vartype, chartype ) ) &&
/**/	 !( compatible( vartype, inttype ) ) ) {
/**/	semanticerror( 67 );
/**/}
} // inputvariable( )

void syntax::readstatement( setofsymbols followers ) {
	accept( readsy );
	accept( leftparent );
	inputvariable( followers );
	while ( symbol == comma ) {
		accept( comma );
		inputvariable( followers );
	}
	accept( rightparent );
} // readstatement( )

void syntax::variable( setofsymbols followers/**/, typentry &vartype/**/ ) {
/**/identry		varid;
/**/typentry	indextype;

	setofsymbols starters = setofsymbols( ident );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
/**/	searchid( spelling, varid, setofidclass( vars ) );
/**/	vartype = varid->idtype;
		accept( ident );
		if ( symbol == leftbracket ) {
/**/		if ( vartype != nil ) {
/**/			if ( vartype->form != arrays ) {
/**/				semanticerror( 61 );
/**/				vartype = nil;
/**/			}
/**/		}
			accept( leftbracket );
			expression( followers + rightbracket/**/, indextype/**/ );
/**/		if (! compatible( indextype, inttype ) ) {
/**/			semanticerror( 62 );
/**/		}
/**/		if ( vartype != nil ) {
/**/			vartype = vartype->elementtype;
/**/		}
			accept( rightbracket );
		}
		endcheck( followers );
	}
} // variable( )

void syntax::assignment( setofsymbols followers ) {
/**/typentry	vartype;
/**/typentry	exptype;

	variable( followers + becomes/**/, vartype/**/ );
	accept( becomes );
	expression( followers/**/, exptype/**/ );
/**/if (! compatible( vartype, exptype ) ) {
/**/	semanticerror( 66 );
/**/}
} // assignment( )

void syntax::expression( setofsymbols followers/**/, typentry &exptype/**/ ) {
/**/typentry	firsttype;
/**/symboltype	oper;

	simpleexpression( followers + relops/**/, exptype/**/ );
	if ( symbol >> relops ) {
/**/	firsttype	= exptype;
/**/	oper		= symbol;
		nextsymbol( );
		simpleexpression( followers/**/, exptype/**/ );
/**/	if (! ( compatible( firsttype, inttype )	&&
/**/		    compatible( exptype, inttype )		||
/**/			compatible( firsttype, chartype )	&&
/**/			compatible( exptype, chartype ) ) ) {
/**/		semanticerror( 65 );
/**/	}
/**/	exptype = booltype;
	}
} // expression( );

void syntax::simpleexpression( setofsymbols followers/**/, typentry &exptype/**/ ) {
/**/bool			signd;
/**/typentry		firsttype;
/**/symboltype		oper;

	setofsymbols starters = factorstarters + signs;
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		if ( symbol >> signs ) { 
/**/		signd = true;
			nextsymbol( );
/**/	} else { signd = false; }
		term( followers + addops/**/, exptype/**/ );
/**/	if ( signd ) {
/**/		if (! compatible( exptype, inttype ) ) {
/**/			semanticerror( 64 );
/**/		}
/**/	}
		while ( symbol >> addops ) {
/**/		firsttype	= exptype;
/**/		oper		= symbol;
			nextsymbol( );
			term( followers + addops/**/, exptype/**/ );
/**/		switch( oper ) {
/**/			case plus: case minus:
/**/			{
/**/				if (! ( compatible( firsttype, inttype ) &&
/**/					    compatible( exptype, inttype ) ) ) {
/**/					semanticerror( 64 );
/**/				}
/**/				exptype = inttype;
/**/			} break;
/**/			case orop:
/**/			{
/**/				if (! ( compatible( firsttype, booltype ) &&
/**/					    compatible( exptype, booltype ) ) ) {
/**/					semanticerror( 63 );
/**/				}
/**/				exptype = booltype;
/**/			} break;
/**/		} // switch
		} // while
		endcheck( followers );
	}
} // simpleexpression( )

void syntax::term( setofsymbols followers/**/, typentry &exptype/**/ ) {
/**/typentry		firsttype;
/**/symboltype		oper;

	factor( followers + factorstarters + mulops/**/, exptype/**/ );
	while ( symbol >> (mulops + factorstarters) ) {
/**/	firsttype	= exptype;
/**/	oper		= symbol;
		if ( symbol >> mulops ) {
			nextsymbol( );
		} else { syntaxerror( times ); }
		factor( followers + factorstarters + mulops/**/, exptype/**/ );
/**/	if ( oper >> mulops ) {
/**/		switch( oper ) {
/**/			case times: case divop:
/**/			{
/**/				if (! ( compatible( firsttype, inttype ) &&
/**/					    compatible( exptype, inttype ) ) ) {
/**/					semanticerror( 64 );
/**/				}
/**/				exptype = inttype;
/**/			} break;
/**/			case andop:
/**/			{
/**/				if (! ( compatible( firsttype, booltype ) &&
/**/					    compatible( exptype, booltype ) ) ) {
/**/					semanticerror( 63 );
/**/				}
/**/				exptype = booltype;
/**/			} break;
/**/		} // switch
/**/	} // if
	} // while
} // term( )

void syntax::factor( setofsymbols followers/**/, typentry &exptype/**/ ) {
	identry	firstid;

	setofsymbols starters = factorstarters;
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		switch( symbol ) {
			case	ident		:	
			{
				searchid( spelling, firstid,
						  setofidclass( 2, vars, consts ) );
	
				switch( firstid->iclass ) {
					case consts	:
/**/				{
/**/					exptype = firstid->idtype;
/**/					accept( ident ); 
/**/				} break;
					case vars	: variable( followers/**/, exptype/**/ ); break;
				}
			} break;
			case	intconst	:
/**/		{
/**/			exptype = inttype;
/**/			accept( intconst ); 
/**/		} break;
			case	charconst	:
/**/		{
/**/			exptype = chartype;
/**/			accept( charconst ); 
/**/		} break;
			case	leftparent	:
			{
				accept( leftparent );
				expression( followers + rightparent/**/, exptype/**/ );
				accept( rightparent );
			} break;
			case	notop		:
			{
				accept( notop );
				factor( followers/**/, exptype/**/ );
/**/			if (! ( compatible( exptype, booltype ) ) ) {
/**/				semanticerror( 63 );
/**/			}
/**/			exptype = booltype;
			} break;
		} // switch
		endcheck( followers );
	}
} // factor( )


void syntax::procdeclaration( setofsymbols followers ) {
...
} // procdeclaration( )
void syntax::procpart( setofsymbols followers ) {
...
} // procpart( )


void syntax::varpart( setofsymbols followers ) {
...
} // varpart( )

void syntax::newvariable( ) {
...
} // newvariable( )
void syntax::addattributes( ) {
...
} // addattributes( )
void syntax::vardeclaration( setofsymbols followers ) {
...
} // vardeclaration( )

void syntax::simpletype( setofsymbols followers, typentry &typenamed ) {
...
} // simpletype( )
void syntax::indexrange( setofsymbols followers, typentry &typefound ) {
...
} // indexrange( )
void syntax::typ( setofsymbols followers, typentry &typefound ) {
...
} // typ( )
