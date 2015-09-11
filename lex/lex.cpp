#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
scan::scan(char *sourcename) :
		source(sourcename) {
	lastoflength[0] = 0;

	strcpy(wordsymbols[1].symbolspelling, "         ");
	wordsymbols[1].symbolvalue = ident;
	lastoflength[1] = 1;

	strcpy(wordsymbols[2].symbolspelling, "IF       ");
	wordsymbols[2].symbolvalue = ifsy;
	strcpy(wordsymbols[3].symbolspelling, "DO       ");
	wordsymbols[3].symbolvalue = dosy;
	strcpy(wordsymbols[4].symbolspelling, "OF       ");
	wordsymbols[4].symbolvalue = ofsy;
	strcpy(wordsymbols[5].symbolspelling, "OR       ");
	wordsymbols[5].symbolvalue = orop;
	strcpy(wordsymbols[6].symbolspelling, "         ");
	wordsymbols[6].symbolvalue = ident;
	lastoflength[2] = 6;

	// ToDo: fill in the table

	strcpy(wordsymbols[26].symbolspelling, "PROCEDURE");
	wordsymbols[26].symbolvalue = procsy;
	strcpy(wordsymbols[27].symbolspelling, "         ");
	wordsymbols[27].symbolvalue = ident;
	lastoflength[9] = 27;

	nextsymbol();
} // scan( )

scan::~scan() {
	// empty
} // ~scan( )

//////////////////////////
//// Public Functions ////
//////////////////////////
void scan::nextsymbol() {
	int i, k, digit;

	// read character by character until next significant character

	while (ch == ' ') {
		nextch();
	}
	// ToDo: handle tabs and comments

	symbolposition = positionnow;

	switch (ch) {
	// Analysis of an identifier or word symbol
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		// ToDo: handle 'a' .. 'z'
	{
		k = 0;
		strcpy(spelling, "         ");

		do {
			if (k < alfalength) {
				spelling[k] = toupper(ch);
				k++;
			}
			nextch();
		} while ((ch >= '0') && (ch <= '9') || (ch >= 'A') && (ch <= 'Z'));
		// ToDo: handle 'a' .. 'z'

		strcpy(wordsymbols[lastoflength[k]].symbolspelling, spelling);
		i = lastoflength[k - 1] + 1;

		while (strcmp(wordsymbols[i].symbolspelling, spelling) != 0) {
			i++;
		}

		symbol = wordsymbols[i].symbolvalue;
	}
		break;
		// Analysis of an integer constant
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9': {
		constant = 0;

		do {
			digit = static_cast<unsigned int>(ch)
					- static_cast<unsigned int>('0');
			if ((constant < (maxinteger / 10))
					|| ((constant == (maxinteger / 10))
							&& (digit <= (maxinteger % 10)))) {
				constant = (10 * constant) + digit;
			} else {
				error(1, positionnow);
				constant = 0;
			}
			nextch();
		} while ((ch >= '0') && (ch <= '9'));

		symbol = intconst;
	}
		break;
		//  Analysis of a character constant
	case '\'': {
		nextch();

		if (ch == '\'') {
			nextch();
			if (ch != '\'') {
				error(2, positionnow);
			}
		}

		constant = static_cast<unsigned int>(ch);
		nextch();

		if (ch != '\'') {
			error(2, positionnow);
		} else {
			nextch();
		}

		symbol = charconst;
	}
		break;
		// 2-character operator/delimiter
	case ':': {
		nextch();
		if (ch == '=') {
			symbol = becomes;
			nextch();
		} else {
			symbol = colon;
		}
	}
		break;
	case '.': {
		// ToDo: . or ..
	}
		break;
	case '<': {
		// ToDo: <, <= or <>
	}
		break;
	case '>': {
		// ToDo: > or >=
	}
		break;
		// 1-character operator delimiter
	case '+': {
		symbol = plus;
		nextch();
	}
		break;
	case '-': { /* ToDo */
	}
		break;
	case '*': { /* ToDo */
	}
		break;
	case '=': { /* ToDo */
	}
		break;
	case '(': { /* ToDo */
	}
		break;
	case ')': { /* ToDo */
	}
		break;
	case '[': { /* ToDo */
	}
		break;
	case ']': { /* ToDo */
	}
		break;
	case ',': { /* ToDo */
	}
		break;
	case ';': { /* ToDo */
	}
		break;
		// Other illegal characters
	default: {
		symbol = othersy;
		nextch();
	}
		break;
	} // switch

} // nextsymbol( )

///////////////////////////
//// Private Functions ////
///////////////////////////
