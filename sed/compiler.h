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
////////////////////////// Scope Tables ///////////////////////////
///////////////////////////////////////////////////////////////////
//																 //
//  The TABLE module organises the creation of, location of, and //
//  storage recovery from, the identifier and type records witch //
//  support semantic analysis.                                   //
//																 //
//  The table is organised as a set of binary trees, one for     //
//  each identifier scope currently open, the nesting of these   //
//  scopes being represented by a stack of scope records.        //
//																 //
//  Scope housekeeping is carried out by an instance of the      //
//  scope envelope created for each block. Within this envelope  //
//  insertion and lookup of identifiers within the table is      //
//  provided by the two procedures "NEWID" and "SEARCHID" .      //
//																 //
//  Recovery from a semantic errors is accomodated within these  //
//  data structures and procedures as follows:                   //
//																 //
//   (1) if NEWID finds an entry for the identifier already in   //
//       the current scope, an error is flagged but a second     //
//       entry is still made (for possible selection by SEARCHID //
//       as below) ;                                             //
//																 //
//   (2) SEARCHID when called is passed a parameter specifying   //
//       the acceptable classes of entry to be found. If the     //
//       first entry encountered for the identifier is not of an //
//       acceptable class searching continues within the current //
//       scope for a possible duplicate entry. If no acceptable  //
//       duplicate is found in the scope a misuse error is       //
//       reported and an anonymous default entry of acceptable   //
//       class is returned ;                                     //
//																 //
//   (3) if SEARCHID fails to find an entry in any scope for the //
//       identifier sought, an undeclared error is reported and  //
//       an entry of acceptable class is created for the ident-  //
//       ifier, with otherwise default attributes .              //
//																 //
//  To facilitate storage recovery the creation of type entries  //
//  is handled by the TABLE module. Each type entry created by   //
//  NEWTYPE is appended in a linear chain to the current block   //
//  scope entry in the display. All storage allocated to table   //
//  entries is recovered at final closure of a block scope .     //
//																 //
//  The TABLE module reports errors with the following codes:    //
//																 //
//       51 ... IDENTIFIER DECLARED TWICE                        //
//       52 ... IDENTIFIER NOT DECLARED                          //
//       53 ... IDENTIFIER OF WRONG CLASS FOR THIS CONTEXT       //
//																 //
///////////////////////////////////////////////////////////////////
////////////////////////// Scope Tables ///////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////
//// Variables & Types ////
///////////////////////////
#define nil 0
// types
typedef setof<symboltype> setofsymbols;

typedef enum typeform {
	scalars, arrays
} typeform;

typedef struct typerec *typentry;
typedef struct typerec {
	typentry	next;
	typeform	form;
	long		indexmin;
	long		indexmax;
	typentry	elementtype;
} typerec;

typedef enum idclass {
	types, consts, vars, procs
} idclass;
#define idclass_len 4

typedef setof<idclass> setofidclass;

typedef struct idrec *identry;
typedef struct idrec {
	alfa		name;
	identry		leftlink;
	identry		rightlink;
	typentry	idtype;
	idclass		iclass;
} idrec;

typedef struct listrec *idlist;
typedef struct listrec {
	identry	id;
	idlist	nextonlist;
} listrec;

typedef struct scoperec *scope;
typedef struct scoperec {
	identry		firstlocal;
	typentry	typechain;
	scope		enclosingscope;
} scoperec;

///////////////////////////
//// Class Declaration ////
///////////////////////////
class table : public scan {
private:	// variables
	scope		localscope;
	identry		defaultentry[idclass_len];
	idclass		c;

public:		// variables
	typentry	vartype;
	struct {
		idlist	head;
		idlist	tail;
	} variablelist;

private:	// functions
	void	disposeids( identry root );
	void	disposetypes( typentry firsttype );
	idclass	mostlikelyof( setofidclass classes );

public:		// functions
	table( char *sourcename );
	~table( );

	void	openscope( );
	void	closescope( );
	void	newid( alfa spelling, identry &entry, idclass classneeded );
	void	searchid( alfa spelling, identry &entry, setofidclass allowableclasses );
	void	newtype( typentry &entry, typeform formneeded );
	void	semanticerror( int code );
};


///////////////////////////////////////////////////////////////////
/////////////// Syntax Analysis with Error Recovery ///////////////
////////////////////// & Data Structures //////////////////////////
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
////////////////////// & Data Structures //////////////////////////
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
////////////////////// & Data Structures //////////////////////////
///////////////////////////////////////////////////////////////////
//																 //
// (C) Semantic analysis and semantic error recovery             //
//																 //
//																 //
//   Semantic analysis and semantic error recovery are           //
//   implemented by "enrichment" of the syntax analyzer with     //
//   semantic interludes. The semantic analysis depends on the   //
//   following data structures and manipulative functions :      //
//																 //
//																 //
//   (1) Identifier entries                                      //
//																 //
//   An entry is recorded for each identifier, either standard   //
//   or program defined, witch may appear in the program being   //
//   compiled. The form of entry used depends on the "class" of  //
//   usage of the identifier and is represented by the record    //
//   type "IDREC". Creation, location and destruction of these   //
//   records is handled by the include-module "TABLE".           //
//																 //
//   Standard identifiers supported by the language are held     //
//   within the table as if declared in a pseudo-block           //
//   enclosing the main program. These entries are created       //
//   on initial entry to the analyzer module.                    //
//																 //
//																 //
//   (2) Type entries                                            //
//																 //
//   All types underlying the data defined by the program being  //
//   compiled are represented by type entries whose form is      //
//   determined by the "form" of the type so represented (i.e.   //
//   scalars, arrays, etc.). Entries are constructed using a     //
//   coresponding variant record type "TYPEREC".                 //
//																 //
//   These type entries are accesed only via the identifier      //
//   table entries for the identifiers, or via the representati- //
//   ons of the data objects (variables, constants, expressions) //
//   whose type they describe. Thus for example all identifier   //
//   table entries have a common field "idtype" witch point to   //
//   an underlying type entry (with an obvious interpretation    //
//   for all classes of identifier other then "PROC").           //
//																 //
//   The type entries representing the standard types supported  //
//   by the language (integer, char, etc.) are created on        //
//   initial entry to the analyzer. These entries are directly   //
//   accesible via pointer variables "INTYPE", "CHARTYPE", etc., //
//   as well as via the identifier entries for "INTEGER",        //
//   "CHAR", etc.                                                //
//																 //
//																 //
//   (3) The function compatible                                 //
//																 //
//   To facilitate type analysis within the semantic analyzer    //
//   a general-purpose boolean function "COMPATIBLE" is provided //
//   to test the compatibility of two types as represented by    //
//   pointers to the corresponding type entries. A result true   //
//   is returned if the types are identical (i.e. the pointers   //
//   point to the same type entry), or strictly equivalent       //
//   (i.e. two distinct type entries of identical form and       //
//   context).                                                   //
//																 //
//   In all situation where the type of data object is not       //
//   determined it is represented by a pointer value 'NIL'.      //
//   The type-checking function "COMPATIBLE" is defined to       //
//   return 'TRUE' if either of its parameters has this value.   //
//   In this way normal type analysis can proceed without a      //
//   preliminary screening for indeterminate types at every      //
//   point at which they might arise.                            //
//																 //
//																 //
//   Semantic errors are reported with the following codes:      //
//																 //
//       61 ... INDEXED VARIABLE MUST BE OF ARRAY TYPE           //
//       62 ... INDEX EXPRESSION MUST BE OF TYPE INTEGER         //
//       63 ... OPERAND MUST BE OF TYPE BOOLEAN                  //
//       64 ... OPERAND MUST BE OF TYPE INTEGER                  //
//       65 ... OPERANDS MUST BOTH BE OF INTEGER, OR BOTH CHAR   //
//       66 ... EXPRESSION MUST BE OF SAME TYPE AS VARIABLE      //
//       67 ... INPUT VARIABLE MUST BE OF TYPE INTEGER OR CHAR   //
//       68 ... OUTPUT VALUE MUST BE OF TYPE INTEGER OR CHAR     //
//       69 ... EXPRESSION MUST BE OF TYPE BOOLEAN               //
//																 //
///////////////////////////////////////////////////////////////////
/////////////// Syntax Analysis with Error Recovery ///////////////
////////////////////// & Data Structures //////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////
//// Variables & Types ////
///////////////////////////
// types


// variables

///////////////////////////
//// Class Declaration ////
///////////////////////////
class syntax : private table {
private:	// variables
	setofsymbols	statstarters;
	setofsymbols	factorstarters;
	setofsymbols	mulops;
	setofsymbols	signs;
	setofsymbols	addops;
	setofsymbols	relops;

/**/typentry	inttype;
/**/typentry	chartype;
/**/typentry	booltype;

public:		// variables

private:	// functions
	void	syntaxerror( symboltype expectedsymbol );
	void	accept( symboltype symbolexpected );
	void	skipto( setofsymbols relevantsymbols );
	void	startcheck( setofsymbols starters, setofsymbols followers );
	void	endcheck( setofsymbols followers );


	void	block( setofsymbols followers );
	void	varpart( setofsymbols followers );
		void	vardeclaration( setofsymbols followers );
			void	newvariable( );
			void	addattributes( );
		void	typ( setofsymbols followers/**/, typentry &typefound/**/ );
			void	simpletype( setofsymbols followers/**/, typentry &typefound/**/ );
			void	indexrange( setofsymbols followers/**/, typentry &typefound/**/ );
	void	procpart( setofsymbols followers );
		void	procdeclaration( setofsymbols followers );
	void	statpart( setofsymbols followers );
		void	compoundstatement( setofsymbols followers );
			void	statement( setofsymbols followers );
				void	variable( setofsymbols followers );
					void	expression( setofsymbols followers );
						void	simpleexpression( setofsymbols followers );
							void	term( setofsymbols followers );
								void	factor( setofsymbols followers );
				void	assignment( setofsymbols followers );
				void	readstatement( setofsymbols followers );
					void	inputvariable( setofsymbols followers );
				void	writestatement( setofsymbols followers );
					void	outputvalue( setofsymbols followers );
				void	ifstatement( setofsymbols followers );
				void	whilestatement( setofsymbols followers );
public:		// functions
	syntax( char *sourcename );
	~syntax( );

	void	programme( );
};