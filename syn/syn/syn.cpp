#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
syntax::syntax( char *sourcename ) : scan( sourcename ) {
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
	accept( programsy );
	accept( ident );
	accept( semicolon );
	block( );
} // programme( )

///////////////////////////
//// Private Functions ////
///////////////////////////
void syntax::syntaxerror( symboltype expectedsymbol ) {
	error( (int)expectedsymbol + 10, symbolposition );
} // syntaxerror( )

void syntax::accept( symboltype symbolexpected ) {
	symbol == symbolexpected	? nextsymbol( )
								: syntaxerror( symbolexpected );
} // accept( )



void syntax::block( ) {
	varpart( );
	procpart( );
	statpart( );
} // block( )

void syntax::statpart( ) {
	compoundstatement( );
} // statpart( )

void syntax::compoundstatement( ) {
	accept( beginsy );
	statement( );
	while ( symbol == semicolon ) {
		accept( semicolon );
		statement( );
	}
	accept( endsy );
} // compoundstatement( );

void syntax::statement( ) {
	if ( symbol >> statstarters ) {
		switch( symbol ) {
			case ident	:	assignment( ); break;
			case beginsy:	compoundstatement( ); break;
			case ifsy	:	ifstatement( ); break;
			case whilesy:	whilestatement( ); break;
			case readsy	:	readstatement( ); break;
			case writesy:	writestatement( ); break;
		} // swtich
	} else {
		syntaxerror( othersy );
	}
} // statement( )

void syntax::ifstatement( ) {
	accept( ifsy );
	expression( );
	accept( thensy );
	statement( );
	if ( symbol == elsesy ) {
		accept( elsesy );
		statement( );
	}
} // ifstatement( )

void syntax::whilestatement( ) {
	accept( whilesy );
	expression( );
	accept( dosy );
	statement( );
} // whilestatement( )

void syntax::outputvalue( ) {
	expression( );
} // outputvalue( )
void syntax::writestatement( ) {
	accept( writesy );
	accept( leftparent );
	outputvalue( );
	while ( symbol == comma ) {
		accept( comma );
		outputvalue( );
	}
	accept( rightparent );
} // writestatement( )

void syntax::inputvariable( ) {
	variable( );
} // inputvariable( )
void syntax::readstatement( ) {
	accept( readsy );
	accept( leftparent );
	inputvariable( );
	while ( symbol == comma ) {
		accept( comma );
		inputvariable( );
	}
	accept( rightparent );
} // readstatement( )

void syntax::variable( ) {
	accept( ident );
	if ( symbol == leftbracket ) {
		accept( leftbracket );
		expression( );
		accept( rightbracket );
	}
} // variable( )

void syntax::assignment( ) {
	variable( );
	accept( becomes );
	expression( );
} // assignment( )

void syntax::expression( ) {
	simpleexpression( );
	if ( symbol >> relops ) {
		nextsymbol( );
		simpleexpression( );
	}
} // expression( );

void syntax::simpleexpression( ) {
	if ( symbol >> signs ) { 
		nextsymbol( );
	}
	term( );
	while ( symbol >> addops ) {
		nextsymbol( );
		term( );
	}
} // simpleexpression( )

void syntax::term( ) {
	factor( );
	while ( symbol >> mulops ) {
		nextsymbol( );
		factor( );
	}
} // term( )

void syntax::factor( ) {
	if ( symbol >> factorstarters ) {
		switch( symbol ) {
			case	ident		:	variable( ); break;
			case	intconst	:	accept( intconst ); break;
			case	charconst	:	accept( charconst ); break;
			case	leftparent	:
			{
				accept( leftparent );
				expression( );
				accept( rightparent );
			} break;
			case	notop		:
			{
				accept( notop );
				factor( );
			} break;
		} // switch
	} else { syntaxerror( othersy ); }
} // factor( )


void syntax::procdeclaration( ) {
	accept( procsy );
	accept( ident );
	accept( semicolon );
	block( );
} // procdeclaration( )
void syntax::procpart( ) {
	while ( symbol == procsy ) {
		procdeclaration( );
		accept( semicolon );
	}
} // procpart( )


void syntax::varpart( ) {
	if ( symbol == varsy ) {
		accept( varsy );
		do {
			vardeclaration( );
			accept( semicolon );
		} while ( symbol == ident );
	}
} // varpart( )

void syntax::vardeclaration( ) {
	accept( ident );
	while ( symbol == comma ) {
		accept( comma );
		accept( ident );
	}
	accept( colon );
	typ( );
} // vardeclaration( )

void syntax::simpletype( ) {
	accept( ident );
} // simpletype( )

void syntax::indexrange( ) {
	accept( intconst );
	accept( thru );
	accept( intconst );
} // indexrange( )
void syntax::typ( ) {
	if ( symbol == ident ) {
		simpletype( );
	} else {
		accept( arraysy );
		accept( leftbracket );
		indexrange( );
		accept( rightbracket );
		accept( ofsy );
		simpletype( );
	}
} // typ( )