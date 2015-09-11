///////////////////////////////////////////////////////////////////
////////////////////// Mini Pascal Compiler ///////////////////////
///////////////////////////////////////////////////////////////////
//////////////////
//// Includes ////
//////////////////
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////
///////////////////////// Source Handler //////////////////////////
///////////////////////////////////////////////////////////////////
//																 //
// The source handler (sourcer) enables text input and source    //
// listing generation through the folowing accesible functions:	 //
//																 //
//																 //
// NEXTCH - This function reads the next source character from	 //
//          the input stream, copies it to the output stream,	 //
//          and leaves its value in the accesible variable CH .	 //
//          The position of the character within the input text	 //
//          is maintained in the accesible variable POSITIONNOW .//
//          Ends of line are transmitted as blank characters.	 //
//																 //
//																 //
// ERROR -  This procedure enables the analysis processes to	 //
//          record error code / TEXTPOSITION pairs for			 //
//          printout during listing generation.					 //
//																 //
//																 //
// Proper initialization and finalization of the source handling //
// process is ensured by the class constructor and destructor.	 //
//																 //
///////////////////////////////////////////////////////////////////
///////////////////////// Source Handler //////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////
//// Variables & Types ////
///////////////////////////
// constants
const	int		maxline	= 81;
const	int		errmax	= 6;

// types
typedef struct textposition {
	int		linenumber;
	int		charnumber;
} textposition;
typedef struct errlistrec {
	int				errorcode;
	textposition	errorposition;
} errlistrec;

///////////////////////////
//// Class Declaration ////
///////////////////////////
class source {
private:	// variables
	// line processing variables
	char			line[maxline];
	int				firstinline;
	int				lastinline;

	// error handling variables
	int				errorcount;
	int				errinx;
	bool			erroroverflow;
	errlistrec		errlist[errmax];

public:		// variables
	// file-stream variables
	ifstream		fin;

	// variables keeping track of sourcer position
	char			ch;
	textposition	positionnow;

private:	// functions
	void	readnextline( );
	void	listerrors( );
	void	listthisline( );

public:		// functions
	source( char * sourcename );
	~source( );

	void	nextch( );
	void	error( int code, textposition position );
};


///////////////////////////////////////////////////////////////////
//////////////////////// Lexical Analysis /////////////////////////
///////////////////////////////////////////////////////////////////
//																 //
// The scan module enables the lexical scanning of symbols in	 //
// the source stream through the accesible procedure NEXTSYMBOL. //
//																 //
// When caled, NEXTSYMBOL scans the next language symbol in the  //
// input stream and returns a representation of it in the		 //
// following accessible variables :								 //
//																 //
//																 //
//   SYMBOL		In all cases SYMBOL represents the symbol		 //
//				scanned, as defined by the type SYMBOLTYPE .	 //
//																 //
//																 //
//   SPELLING	When SYMBOL = IDENT , SPELLING holds the		 //
//				(significant) characters of the identifier		 //
//				scanned .										 //
//																 //
//																 //
//   CONSTANT	When SYMBOL = INTCONST or CHARCONST , CONSTANT   //
//				holds the integer representation of the			 //
//				constant .										 //
//																 //
//																 //
// The starting position of the symbol scanned is left in		 //
// the accessible variable SYMBOLPOSITION .						 //
//																 //
// The scanner (lexical analizer) report errors with the		 //
// folowing codes :												 //
//																 //
//  1 .... INTEGER CONSTANT TOO LARGE							 //
//  2 .... CHARACTER CONSTANT INCOMPLETE						 //
//																 //
///////////////////////////////////////////////////////////////////
//////////////////////// Lexical Analysis /////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////
//// Variables & Types ////
///////////////////////////
// constants
const	int		alfalength		= 9;
//const	int		alfalength		= 10;
const	int		nowordsymbols	= 28;
const	int		maxinteger		= 32767;

// types
typedef char alfa[alfalength+1]; // !!! +1 for the string terminator -> \0
typedef enum symboltype {
	ident, intconst, charconst,
	notop, andop, orop,
	times, divop, plus, minus,
	ltop, leop, geop, gtop, neop, eqop,
	rightparent, leftparent, leftbracket, rightbracket,
	comma, semicolon, period, colon, becomes, thru,
	programsy, varsy, procsy, arraysy, ofsy,
	beginsy, endsy, ifsy, thensy, elsesy, whilesy, dosy,
	readsy, writesy,
	othersy
} symboltype;
typedef struct wordsymbolsrec {
	symboltype	symbolvalue; 
	alfa		symbolspelling;
} wordsymbolsrec;

///////////////////////////
//// Class Declaration ////
///////////////////////////
class scan : public source {
private:	// variables
	// variables for handling symbols and scans
	wordsymbolsrec	wordsymbols[nowordsymbols];
	int				lastoflength[alfalength+1];

public:		// variables
	// variables for handling symbols and scans
	textposition	symbolposition;
	symboltype		symbol;
	int				constant;
	alfa			spelling;

private:	// functions

public:		// functions
	scan( char *sourcename );
	~scan( );

	void	nextsymbol( );
};
