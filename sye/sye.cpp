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
	block( /**/setofsymbols( period )/**/ );
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

/**/void syntax::skipto( setofsymbols relevantsymbols ) {
/**/	while ( !( symbol >> relevantsymbols ) ) {
/**/		nextsymbol( );
/**/	}
/**/}

/**/void syntax::startcheck( setofsymbols starters, setofsymbols followers ) {
/**/	if ( !( symbol >> starters ) ) {
/**/		syntaxerror( othersy );
/**/		skipto( starters + followers );
/**/	}
/**/}


/**/void syntax::endcheck( setofsymbols followers ) {
/**/	if ( !( symbol >> followers ) ) {
/**/		syntaxerror( othersy );
/**/		skipto( followers );
/**/	}
/**/}



void syntax::block( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( 3, varsy, procsy, beginsy );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		varpart( /**/setofsymbols( 2, procsy, beginsy )/**/ );
		procpart( /**/setofsymbols( beginsy )/**/ );
		statpart( /**/followers/**/ );
/**/	endcheck( followers );
/**/}
} // block( )

void syntax::statpart( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( beginsy );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		compoundstatement( /**/followers/**/ );
/**/	endcheck( followers );
/**/}
} // statpart( )

void syntax::compoundstatement( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( beginsy );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( beginsy );
		statement( /**/statstarters + semicolon 
					+ endsy - ident + followers/**/ );
/**/	while ( symbol >> (statstarters + semicolon - ident) ) {
			accept( semicolon );
			statement( /**/statstarters + semicolon 
						+ endsy - ident + followers/**/ );
		}
		accept( endsy );
/**/	endcheck( followers );
/**/}
} // compoundstatement( );

void syntax::statement( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = statstarters;
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		switch( symbol ) {
			case ident	:	assignment( /**/followers/**/ ); break;
			case beginsy:	compoundstatement( /**/followers/**/ ); break;
			case ifsy	:	ifstatement( /**/followers/**/ ); break;
			case whilesy:	whilestatement( /**/followers/**/ ); break;
			case readsy	:	readstatement( /**/followers/**/ ); break;
			case writesy:	writestatement( /**/followers/**/ ); break;
		} // swtich
/**/	endcheck( followers );
/**/}
} // statement( )

void syntax::ifstatement( /**/setofsymbols followers/**/ ) {
	accept( ifsy );
	expression( /**/followers + thensy + elsesy/**/ );
	accept( thensy );
	statement( /**/followers + elsesy/**/ );
	if ( symbol == elsesy ) {
		accept( elsesy );
		statement( /**/followers/**/ );
	}
} // ifstatement( )

void syntax::whilestatement( /**/setofsymbols followers/**/ ) {
	accept( whilesy );
	expression( /**/followers + dosy/**/ );
	accept( dosy );
	statement( /**/followers/**/ );
} // whilestatement( )

void syntax::outputvalue( /**/setofsymbols followers/**/ ) {
	expression( /**/followers + comma + rightparent/**/ );
} // outputvalue( )
void syntax::writestatement( /**/setofsymbols followers/**/ ) {
	accept( writesy );
	accept( leftparent );
	outputvalue( /**/followers/**/ );
	while ( symbol == comma ) {
		accept( comma );
		outputvalue( /**/followers/**/ );
	}
	accept( rightparent );
} // writestatement( )

void syntax::inputvariable( /**/setofsymbols followers/**/ ) {
	variable( /**/followers + comma + rightparent/**/ );
} // inputvariable( )
void syntax::readstatement( /**/setofsymbols followers/**/ ) {
	accept( readsy );
	accept( leftparent );
	inputvariable( /**/followers/**/ );
	while ( symbol == comma ) {
		accept( comma );
		inputvariable( /**/followers/**/ );
	}
	accept( rightparent );
} // readstatement( )

void syntax::variable( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( ident );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( ident );
		if ( symbol == leftbracket ) {
			accept( leftbracket );
			expression( /**/followers + rightbracket/**/ );
			accept( rightbracket );
		}
/**/	endcheck( followers );
/**/}
} // variable( )

void syntax::assignment( /**/setofsymbols followers/**/ ) {
	variable( /**/followers + becomes/**/ );
	accept( becomes );
	expression( /**/followers/**/ );
} // assignment( )

void syntax::expression( /**/setofsymbols followers/**/ ) {
	simpleexpression( /**/followers + relops/**/ );
	if ( symbol >> relops ) {
		nextsymbol( );
		simpleexpression( /**/followers/**/ );
	}
} // expression( );

void syntax::simpleexpression( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = factorstarters + signs;
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		if ( symbol >> signs ) { 
			nextsymbol( );
		}
		term( /**/followers + addops/**/ );
		while ( symbol >> addops ) {
			nextsymbol( );
			term( /**/followers + addops/**/ );
		}
/**/	endcheck( followers );
/**/}
} // simpleexpression( )

void syntax::term( /**/setofsymbols followers/**/ ) {
	factor( /**/followers + factorstarters + mulops/**/ );
/**/while ( symbol >> (mulops + factorstarters) ) {
/**/	if ( symbol >> mulops ) {
/**/		nextsymbol( );
/**/	} else { syntaxerror( times ); }
/**/	factor( followers + factorstarters + mulops );
/**/}
} // term( )

void syntax::factor( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = factorstarters;
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		switch( symbol ) {
			case	ident		:	variable( /**/followers/**/ ); break;
			case	intconst	:	accept( intconst ); break;
			case	charconst	:	accept( charconst ); break;
			case	leftparent	:
			{
				accept( leftparent );
				expression( /**/followers + rightparent/**/ );
				accept( rightparent );
			} break;
			case	notop		:
			{
				accept( notop );
				factor( /**/followers/**/ );
			} break;
		} // switch
/**/	endcheck( followers );
/**/}
} // factor( )


void syntax::procdeclaration( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( procsy );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( procsy );
		accept( ident );
		accept( semicolon );
		block( /**/followers/**/ );
/**/	endcheck( followers );
/**/}
} // procdeclaration( )
void syntax::procpart( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = followers + procsy;
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		while ( symbol == procsy ) {
			procdeclaration( /**/followers + procsy + semicolon/**/ );
			accept( semicolon );
		}
/**/	endcheck( followers );
/**/}
} // procpart( )


void syntax::varpart( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = followers + varsy;
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {	
		if ( symbol == varsy ) {
			accept( varsy );
			do {
				vardeclaration( /**/followers + semicolon/**/ );
				accept( semicolon );
			} while ( symbol == ident );
		}
/**/	endcheck( followers );
/**/}
} // varpart( )

void syntax::vardeclaration( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( 3, ident, comma, colon );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( ident );
		while ( symbol == comma ) {
			accept( comma );
			accept( ident );
		}
		accept( colon );
		typ( /**/followers/**/ );
/**/	endcheck( followers );
/**/}
} // vardeclaration( )

void syntax::simpletype( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( ident );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( ident );
/**/	endcheck( followers );
/**/}
} // simpletype( )
void syntax::indexrange( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( 2, intconst, thru );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		accept( intconst );
		accept( thru );
		accept( intconst );
/**/	endcheck( followers );
/**/}
} // indexrange( )
void syntax::typ( /**/setofsymbols followers/**/ ) {
/**/setofsymbols starters = setofsymbols( 2, ident, arraysy );
/**/startcheck( starters, followers );
/**/if ( symbol >> starters ) {
		if ( symbol == ident ) {
			simpletype( /**/followers/**/ );
		} else {
			accept( arraysy );
			accept( leftbracket );
			indexrange( /**/followers + rightbracket + ofsy/**/ );
			accept( rightbracket );
			accept( ofsy );
			simpletype( /**/followers/**/ );
		}
/**/	endcheck( followers );
/**/}
} // typ( )