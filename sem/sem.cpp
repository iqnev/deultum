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
	identry	entry;
	
	openscope( );
	newtype( inttype, scalars );
	newtype( chartype, scalars );
	newtype( booltype, scalars );
	newid( "INTEGER  ", entry, types );
	entry->idtype = inttype;
	newid( "CHAR     ", entry, types );
	entry->idtype = chartype;
	newid( "BOOLEAN  ", entry, types );
	entry->idtype = booltype;
	newid( "FALSE    ", entry, consts );
	entry->idtype = booltype;
	newid( "TRUE     ", entry, consts );
	entry->idtype = booltype;
	accept( programsy );
	accept( ident );
	accept( semicolon );
	block( setofsymbols( period ) );
	closescope( );
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
	error( (int)expectedsymbol + 10, symbolposition );
} // syntaxerror( )

void syntax::accept( symboltype symbolexpected ) {
	symbol == symbolexpected	? nextsymbol( )
								: syntaxerror( symbolexpected );
} // accept( )

void syntax::skipto( setofsymbols relevantsymbols ) {
	while ( !( symbol >> relevantsymbols ) ) {
		nextsymbol( );
	}
}

void syntax::startcheck( setofsymbols starters, setofsymbols followers ) {
	if ( !( symbol >> starters ) ) {
		syntaxerror( othersy );
		skipto( starters + followers );
	}
}


void syntax::endcheck( setofsymbols followers ) {
	if ( !( symbol >> followers ) ) {
		syntaxerror( othersy );
		skipto( followers );
	}
}



void syntax::block( setofsymbols followers ) {
	openscope( );
	setofsymbols starters = setofsymbols( 3, varsy, procsy, beginsy );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		varpart( setofsymbols( 2, procsy, beginsy ) );
		procpart( setofsymbols( beginsy ) );
		statpart( followers );
		endcheck( followers );
	}
	closescope( );
} // block( )

void syntax::statpart( setofsymbols followers ) {
	setofsymbols starters = setofsymbols( beginsy );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		compoundstatement( followers );
		endcheck( followers );
	}
} // statpart( )

void syntax::compoundstatement( setofsymbols followers ) {
	setofsymbols starters = setofsymbols( beginsy );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		accept( beginsy );
		statement( statstarters + semicolon 
					+ endsy - ident + followers );
		while ( symbol >> (statstarters + semicolon - ident) ) {
			accept( semicolon );
			statement( statstarters + semicolon 
						+ endsy - ident + followers );
		}
		accept( endsy );
		endcheck( followers );
	}
} // compoundstatement( );

void syntax::statement( setofsymbols followers ) {
	identry	firstid;

	setofsymbols starters = statstarters;
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		switch( symbol ) {
			case ident	:	
			{
				searchid( spelling, firstid, 
					      setofidclass( 2, procs, vars ) );
	
				if ( firstid->iclass == vars ) {
					assignment( followers ); 
				} else { accept( ident ); }
			} break;
			case beginsy:	compoundstatement( followers ); break;
			case ifsy	:	ifstatement( followers ); break;
			case whilesy:	whilestatement( followers ); break;
			case readsy	:	readstatement( followers ); break;
			case writesy:	writestatement( followers ); break;
		} // swtich
		endcheck( followers );
	}
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
	identry procid;
	alfa	procname;

	setofsymbols starters = setofsymbols( procsy );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		accept( procsy );
		if ( symbol == ident ) {
			strcpy( procname, spelling );
		} else { strcpy( procname, "????????" ); }
		newid( procname, procid, procs );
		accept( ident );
		accept( semicolon );
		block( followers );
		endcheck( followers );
	}
} // procdeclaration( )
void syntax::procpart( setofsymbols followers ) {
	setofsymbols starters = followers + procsy;
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		while ( symbol == procsy ) {
			procdeclaration( followers + procsy + semicolon );
			accept( semicolon );
		}
		endcheck( followers );
	}
} // procpart( )


void syntax::varpart( setofsymbols followers ) {
	setofsymbols starters = followers + varsy;
	startcheck( starters, followers );
	if ( symbol >> starters ) {	
		if ( symbol == varsy ) {
			accept( varsy );
			do {
				vardeclaration( followers + semicolon );
				accept( semicolon );
			} while ( symbol == ident );
		}
		endcheck( followers );
	}
} // varpart( )

void syntax::newvariable( ) {
	identry	varentry;
	idlist	listentry;

	if ( symbol == ident ) {
		newid( spelling, varentry, vars );

		listentry = new listrec;

		listentry->id			= varentry;
		listentry->nextonlist	= nil;

		if ( variablelist.head == nil ) {
			variablelist.head = listentry;
		} else { variablelist.tail->nextonlist = listentry; }
		variablelist.tail = listentry;
	}
} // newvariable( )
void syntax::addattributes( ) {
	idlist	listentry;
	idlist	oldentry;
	
	listentry = variablelist.head;
	while ( listentry != nil ) {
		listentry->id->idtype = vartype;
		oldentry = listentry;
		listentry = listentry->nextonlist;
		delete oldentry;
	}
} // addattributes( )
void syntax::vardeclaration( setofsymbols followers ) {
	setofsymbols starters = setofsymbols( 3, ident, comma, colon );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		variablelist.head = nil;
		newvariable( );
		accept( ident );
		while ( symbol == comma ) {
			accept( comma );
			newvariable( );
			accept( ident );
		}
		accept( colon );
		typ( followers, vartype );
		addattributes( );
		endcheck( followers );
	}
} // vardeclaration( )

void syntax::simpletype( setofsymbols followers, typentry &typenamed ) {
	identry	type_id;

	setofsymbols starters = setofsymbols( ident );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		searchid( spelling, type_id, setofidclass( types ) );
		typenamed = type_id->idtype;
		accept( ident );
		endcheck( followers );
	}
} // simpletype( )
void syntax::indexrange( setofsymbols followers, typentry &typefound ) {
	setofsymbols starters = setofsymbols( 2, intconst, thru );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		typefound->indexmin = constant;
		accept( intconst );
		accept( thru );
		accept( intconst );
		typefound->indexmax = constant;
		endcheck( followers );
	}
} // indexrange( )
void syntax::typ( setofsymbols followers, typentry &typefound ) {
	setofsymbols starters = setofsymbols( 2, ident, arraysy );
	startcheck( starters, followers );
	if ( symbol >> starters ) {
		if ( symbol == ident ) {
			simpletype( followers, typefound );
		} else {
			newtype( typefound, arrays );
			accept( arraysy );
			accept( leftbracket );
			indexrange( followers + rightbracket + ofsy, typefound );
			accept( rightbracket );
			accept( ofsy );
			simpletype( followers, typefound->elementtype );
		}
		endcheck( followers );
	}
} // typ( )