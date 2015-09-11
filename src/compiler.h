
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


