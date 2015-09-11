#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
table::table( char *sourcename ) : scan( sourcename ) {
	localscope = nil;

	for (c=types; c<=procs; c=(idclass)((int)c+1)) {
		defaultentry[c] = new idrec;

		strcpy( defaultentry[c]->name, "        " );

		defaultentry[c]->idtype	= nil;
		defaultentry[c]->iclass = c;
	}
} // table( )

table::~table( ) {
	// empty
} // ~table( )

//////////////////////////
//// Public Functions ////
//////////////////////////
void table::semanticerror( int code ) {
	error( code, symbolposition );
}

void table::openscope( ) {
	scope	newscope = new scoperec;

	newscope->firstlocal		= nil;
	newscope->typechain			= nil;
	newscope->enclosingscope	= localscope;

	localscope = newscope;
} // openscope_table( )

void table::disposeids( identry root ) {
	if ( root != nil ) {
		disposeids( root->leftlink );
		disposeids( root->rightlink );

		delete root;
	}
} // disposeids( )
void table::disposetypes( typentry firsttype ) {
	typentry thistype;
	typentry nexttype;

	nexttype = firsttype;
	while ( nexttype != nil ) {
		thistype = nexttype;
		nexttype = thistype->next;
		switch( thistype->form ) {
			case scalars: delete thistype; break;
			case arrays	: delete thistype; break;
		}
	} // while
} // disposetypes( )
void table::closescope( ) {
	scope	oldscope;

	oldscope	= localscope;
	localscope	= localscope->enclosingscope;

	disposeids( oldscope->firstlocal );
	disposetypes( oldscope->typechain );

	delete oldscope;
} // closescope( )

void table::newid( alfa spelling, identry &entry, idclass classneeded ) {
	scope		thisscope;
	identry		newentry;
	identry		thisentry;
	identry		lastentry;
	bool		lefttaken;

	newentry = new idrec;

    // Set name, class, and default attributes
	strcpy( newentry->name, spelling );

	newentry->idtype	= nil;
	newentry->leftlink	= nil;
	newentry->rightlink = nil;
	newentry->iclass	= classneeded;

	// Enter in current scope
	thisscope = localscope;
	thisentry = thisscope->firstlocal;
	if ( thisentry == nil ) {
		thisscope->firstlocal = newentry;
	} else {
		do {
			lastentry = thisentry;
			if ( strcmp( thisentry->name, spelling ) > 0 ) {
				thisentry = thisentry->leftlink;
				lefttaken = true;
			} else if ( strcmp( thisentry->name, spelling ) < 0 ) {
				thisentry = thisentry->rightlink;
				lefttaken = false;
			} else {
				semanticerror( 51 );
				thisentry = thisentry->rightlink;
				lefttaken = false;
			}
		} while ( thisentry != nil );

		if ( lefttaken ) {
			lastentry->leftlink = newentry;
		} else { lastentry->rightlink = newentry; }
	} // if (else)

	entry = newentry;
} // newid( )

idclass table::mostlikelyof( setofidclass classes ) {
	if ( consts /*vars*/ >> classes ) {
		return consts /*vars*/;
	} else if ( procs >> classes ) {
		return procs;
	} else if ( types >> classes ) {
		return types;
	} else { return vars /*consts*/; }
} // mostlikelyof( )

void table::searchid( alfa spelling, identry &entry, setofidclass allowableclasses ) {
	identry		thisentry;
//	identry		lastentry;
	bool		misused;
//	bool		lefttaken;
	scope		thisscope;

	misused = false;

	thisscope = localscope;
	do {
		thisentry = thisscope->firstlocal;

		while ( thisentry != nil ) {
			if ( strcmp( thisentry->name, spelling ) > 0 ) {
				thisentry = thisentry->leftlink;
			} else if ( strcmp( thisentry->name, spelling ) < 0 ) {
				thisentry = thisentry->rightlink;
			} else if ( thisentry->iclass >> allowableclasses ) {
				goto stop;
			} else {
				misused		= true;
				thisentry	= thisentry->rightlink;
			}
		} // while

		if ( misused ) {
			semanticerror( 53 );
			thisentry = defaultentry[mostlikelyof(allowableclasses)];
            goto stop;
		}

		thisscope = thisscope->enclosingscope;
	} while ( thisscope != nil );

	semanticerror( 52 );
	newid( spelling, thisentry, mostlikelyof( allowableclasses ) );

	stop:  entry = thisentry;
} // searchid( )

void table::newtype( typentry &entry, typeform formneeded ) {
	scope		thisscope;
	typentry	newentry;

	switch( formneeded ) {
		case scalars:
		{
			newentry = new typerec;

			newentry->form = scalars;
		} break;

		case arrays:
		{
			newentry = new typerec;

			newentry->form			= arrays;
			newentry->indexmin		= 0;
			newentry->indexmax		= 1;
			newentry->elementtype	= nil;
		} break;
	}

	thisscope = localscope;

	newentry->next			= thisscope->typechain;
	thisscope->typechain	= newentry;

	entry = newentry;
} // newtype( )