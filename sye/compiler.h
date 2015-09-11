
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
// the source stream through the accesible function NEXTSYMBOL.	 //
//																 //
// When called, NEXTSYMBOL scans the next language symbol in the //
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
const	int		nowordsymbols	= 27;
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
	wordsymbolsrec	wordsymbols[nowordsymbols+1];
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


///////////////////////////////////////////////////////////////////
////////////////////////// SetOf Class ////////////////////////////
///////////////////////////////////////////////////////////////////
//																 //
// A simple class that emulates sets. It's main purpose is for	 //
// usage with (self-)enumerations. It is strongly adviced that	 //
// it is not used with other types/classes, only enumerations.	 //
//																 //
///////////////////////////////////////////////////////////////////
////////////////////////// SetOf Class ////////////////////////////
///////////////////////////////////////////////////////////////////
#include "..\setof.h"

///////////////////////////////////////////////////////////////////
/////////////// Syntax Analysis with Error Recovery ///////////////
///////////////////////////////////////////////////////////////////
//																 //
// (A) Syntax analysis											 //
//																 //
//   Sintax analysis of Mini-Pascal programs is implemented		 //
//   as a set of recursive descent functions. These functions	 //
//   are based on the syntax rules given in the language defini- //
//   tion and are nested as tightly as the mutual interaction	 //
//   permits. The order, names, and nesting of the functions	 //
//   is as follows:												 //
//																 //
//	PROGRAMME													 //
//	    BLOCK													 //
//			VARPART												 //
//				VARDECLARATION									 //
//				TYP												 //
//					SIMPLETYPE									 //
//					INDEXRANGE									 //
//			PROCPART											 //
//			    PROCDECLARATION									 //
//			STATPART											 //
//			    COMPOUNDSTATEMENT								 //
//					STATEMENT									 //
//						VARIABLE								 //
//						EXPRESSION								 //
//							SIMPLEEXPRESSION					 //
//								TERM							 //
//									FACTOR						 //
//						ASSIGNMENT								 //
//						READSTATEMENT							 //
//							INPUTVARIABLE						 //
//						WRITESTATEMENT							 //
//							OUTPUTVALUE							 //
//						IFSTATEMENT								 //
//						WHILESTATEMENT							 //
//																 //
//																 //
//   The syntax analyzers are writen on the assumption that the	 //
//   next syntactic goal can always be selected by inspection of //
//   (at most) the next incoming symbol ( i.e. that the under-	 //
//   lying gramar is LL(1) ). This is not so at the following	 //
//   points in the syntax rules actually used:					 //
//																 //
//	1. a statement begining with an identifier may be			 //
//	   either an assignment or a procedure call ;				 //
//	2. a factor beginning with an identifier may be				 //
//	   either a variable or a constant.							 //
//																 //
//   In case 1 to resolve the choise on a purely syntactic		 //
//   basis would require a distortion of the syntax rules.		 //
//   Choice 2 cannot be syntactically resolved in some cases	 //
//   however if parallel semantic analysis is assumed (as in	 //
//   the case of this compiler) these choises can be resolwed	 //
//   without sintax distortion, by inspection of the current	 //
//   semantic attributes of the identifier involved. For this	 //
//   reason syntactic resolution of these choises is not used.	 //
//																 //
//																 //
//   The analyzer generates syntax error codes with the			 //
//   following meanings:										 //
//																 //
//	10 .... SYMBOL EXPECTED WAS IDENTIFIER						 //
//	11 .... SYMBOL EXPECTED WAS INTEGER CONSTANT				 //
//	12 .... SYMBOL EXPECTED WAS CHARACTER CONSTANT				 //
//	13 .... ...													 //
//																 //
//   I.e. one value for each of the values of symboltype.		 //
//   The final value (int)othersy+10 is used to mean			 //
//																 //
//	NN .... UNEXPECTED SYMBOL									 //
//																 //
//																 //
///////////////////////////////////////////////////////////////////
/////////////// Syntax Analysis with Error Recovery ///////////////
///////////////////////////////////////////////////////////////////
//																 //
// (B) Syntactic Error Recovery                                  //
//																 //
//   Recovery in the syntax analysis process following the       //
//   discovery of a syntax error in incorporated into the        //
//   syntax functions on the following basis:                    //
//																 //
//       1. each function when called is passed an actual		 //
//          parameter witch is a set of symbols with are         //
//          possible followers of the string witch is should     //
//          scan. These followers normally include:              //
//																 //
//               (a) all symbols witch may legitimately follow   //
//                   the string to be scanned ;                  //
//                                                               //
//               (b) such additional symbols as a superior       //
//                   (calling) procedure may wish to handle      //
//                   in the even of error recovery ;             //
//																 //
//       2. when entered the function may ensure that the        //
//          current symbol is an acceptable starter for the      //
//          string to be scanned, and if not scan forward        //
//          until such a symbol is found (subject to 4. below);  //
//																 //
//       3. when calling a subsidiary syntax function the        //
//          function passes on as followers its own followers    //
//          plus those symbols if any which it may determine     //
//          as followers for the subsring to be scanned;         //
//																 //
//       4. to recover from a syntax error the function may      //
//          scan over (skip) any symbol provided it is not       //
//          contained in the followers passed to it;             //
//																 //
//       5. on exit the syntax function ensures thet the         //
//          current symbol contained in the followers passed     //
//          to it, flagging a terminal error and skipping        //
//          if this is not initialy the case.                    //
//																 //
//   Test 2 and 5 are implemented by calling of the context      //
//   checking functions STARTCHECK and ENDCHECK within each      //
//   syntax function.                                            //
//																 //
///////////////////////////////////////////////////////////////////
/////////////// Syntax Analysis with Error Recovery ///////////////
///////////////////////////////////////////////////////////////////
///////////////////////////
//// Variables & Types ////
///////////////////////////
// types
typedef setof<symboltype> setofsymbols;

// variables

///////////////////////////
//// Class Declaration ////
///////////////////////////
class syntax : private scan {
private:	// variables
	setofsymbols	statstarters;
	setofsymbols	factorstarters;
	setofsymbols	mulops;
	setofsymbols	signs;
	setofsymbols	addops;
	setofsymbols	relops;

public:		// variables

private:	// functions
	void	syntaxerror( symboltype expectedsymbol );
	void	accept( symboltype symbolexpected );
/**/void	skipto( setofsymbols relevantsymbols );
/**/void	startcheck( setofsymbols starters, setofsymbols followers );
/**/void	endcheck( setofsymbols followers );


	void	block( /**/setofsymbols followers/**/ );
	void	varpart( /**/setofsymbols followers/**/ );
		void	vardeclaration( /**/setofsymbols followers/**/ );
		void	typ( /**/setofsymbols followers/**/ );
			void	simpletype( /**/setofsymbols followers/**/ );
			void	indexrange( /**/setofsymbols followers/**/ );
	void	procpart( /**/setofsymbols followers/**/ );
		void	procdeclaration( /**/setofsymbols followers/**/ );
	void	statpart( /**/setofsymbols followers/**/ );
		void	compoundstatement( /**/setofsymbols followers/**/ );
			void	statement( /**/setofsymbols followers/**/ );
				void	variable( /**/setofsymbols followers/**/ );
					void	expression( /**/setofsymbols followers/**/ );
						void	simpleexpression( /**/setofsymbols followers/**/ );
							void	term( /**/setofsymbols followers/**/ );
								void	factor( /**/setofsymbols followers/**/ );
				void	assignment( /**/setofsymbols followers/**/ );
				void	readstatement( /**/setofsymbols followers/**/ );
					void	inputvariable( /**/setofsymbols followers/**/ );
				void	writestatement( /**/setofsymbols followers/**/ );
					void	outputvalue( /**/setofsymbols followers/**/ );
				void	ifstatement( /**/setofsymbols followers/**/ );
				void	whilestatement( /**/setofsymbols followers/**/ );
public:		// functions
	syntax( char *sourcename );
	~syntax( );

	void	programme( );
};
