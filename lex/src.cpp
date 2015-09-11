#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
source::source( char *sourcename ) {
	fin.open( sourcename );

	if (! fin.is_open( ) ) {
		cerr << "*****   ERROR: Could not open file: " 
			 << sourcename << endl;
		exit( EXIT_FAILURE );
	}

	cout << "LISTING PRODUCED BY MINI-PASCAL COMPILER  MPC-1";
	cout << endl << endl;

    readnextline( );
	
	positionnow.linenumber = 1;
	positionnow.charnumber = firstinline;

	ch = line[positionnow.charnumber];

    errorcount		= 0;
    errinx			= 0;
    erroroverflow	= false;
} // source( )

source::~source( ) {
	listthisline( );

	cout << endl << endl;
	cout << "COMPILATION COMPLETED :";
	errorcount == 0	? cout << " NO" 
					: cout << setw(5) << errorcount;
	cout << " ERROR(S) REPORTED." << endl;

	fin.close( );
} // ~source( )

//////////////////////////
//// Public Functions ////
//////////////////////////
void source::nextch( ) {
	if ( positionnow.charnumber == lastinline ) {
		listthisline( );
		readnextline( );

		positionnow.charnumber = firstinline;
		positionnow.linenumber++;
	} else {
		positionnow.charnumber++;
	}

	ch = line[positionnow.charnumber];
} // nextch( )

void source::error( int code, textposition position ) {
	if ( errinx == errmax ) {
		erroroverflow = true;
	} else {
		errlist[errinx].errorcode = code;
		errlist[errinx].errorposition = position;
		errinx++;
	}
} // error( )

///////////////////////////
//// Private Functions ////
///////////////////////////
void source::readnextline( ) {
	char	inChar;
	int		i = 0;

	firstinline = 0;
	
	while ( fin.get( inChar ) && ( inChar != '\n' ) ) {
		line[i] = inChar;
		i++;
		if ( i > maxline ) {
			cerr << "*****   ERROR: Line too long! Aborting..." << endl;
			exit( EXIT_FAILURE );
		}
	}

	fin.eof( )	? line[i] = 0
				: line[i] = ' ';

	lastinline = i;
} // readnextline( )

void source::listerrors( ) {
	errorcount += errinx;

	for (int k=0; k<errinx; k++) {
		cout << "*****   ";
		if ( errlist[k].errorposition.linenumber != positionnow.linenumber ) {
			int tempVar = errlist[k].errorposition.charnumber - firstinline + 1;
			cout << "ERROR" 
				 << setw(10) << errlist[k].errorcode 
				 << " AT CHARACTER"
				 << setw(3) << tempVar 
				 << " OF LINE" 
				 << setw(6) << errlist[k].errorposition.linenumber;
		} else {
			int tempVar = errlist[k].errorposition.charnumber - firstinline + 6;
			cout << setw(tempVar) << "^ERROR" 
				 << setw(4) << errlist[k].errorcode;
		}
		cout << endl;
	}

	if ( erroroverflow ) {
		cout << "*****   FURTHER ERRORS SUPPRESSED"; 
		cout << endl;
	}
    cout << endl;

    errinx			= 0;
    erroroverflow	= false;
} // listerrors( )

void source::listthisline( ) {
	cout << setw(5) << positionnow.linenumber << "   ";
	for (int i=firstinline; i<=lastinline; i++) {
		cout << line[i];
	}
    cout << endl;

	if ( errinx > 0 ) { 
		listerrors( ); 
		cin.get();
	}
} // listthisline( )