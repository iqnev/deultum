#include "compiler.h"

//////////////////////////////////
//// Constructor & Destructor ////
//////////////////////////////////
scan::scan(char *sourcename) :
		source(sourcename) {
	strcpy(wordsymbols[0].symbolspelling, "         ");
	wordsymbols[0].symbolvalue = ident;
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

	strcpy(wordsymbols[7].symbolspelling, "END      ");
	wordsymbols[7].symbolvalue = endsy;
	strcpy(wordsymbols[8].symbolspelling, "VAR      ");
	wordsymbols[8].symbolvalue = varsy;
	strcpy(wordsymbols[9].symbolspelling, "DIV      ");
	wordsymbols[9].symbolvalue = divop;
	strcpy(wordsymbols[10].symbolspelling, "AND      ");
	wordsymbols[10].symbolvalue = andop;
	strcpy(wordsymbols[11].symbolspelling, "NOT      ");
	wordsymbols[11].symbolvalue = notop;
	strcpy(wordsymbols[12].symbolspelling, "         ");
	wordsymbols[12].symbolvalue = ident;
	lastoflength[3] = 12;

	strcpy(wordsymbols[13].symbolspelling, "THEN     ");
	wordsymbols[13].symbolvalue = thensy;
	strcpy(wordsymbols[14].symbolspelling, "ELSE     ");
	wordsymbols[14].symbolvalue = elsesy;
	strcpy(wordsymbols[15].symbolspelling, "READ     ");
	wordsymbols[15].symbolvalue = readsy;
	strcpy(wordsymbols[16].symbolspelling, "         ");
	wordsymbols[16].symbolvalue = ident;
	lastoflength[4] = 16;

	strcpy(wordsymbols[17].symbolspelling, "BEGIN    ");
	wordsymbols[17].symbolvalue = beginsy;
	strcpy(wordsymbols[18].symbolspelling, "WHILE    ");
	wordsymbols[18].symbolvalue = whilesy;
	strcpy(wordsymbols[19].symbolspelling, "ARRAY    ");
	wordsymbols[19].symbolvalue = arraysy;
	strcpy(wordsymbols[20].symbolspelling, "WRITE    ");
	wordsymbols[20].symbolvalue = writesy;
	strcpy(wordsymbols[21].symbolspelling, "         ");
	wordsymbols[21].symbolvalue = ident;
	lastoflength[5] = 21;

	strcpy(wordsymbols[22].symbolspelling, "         ");
	wordsymbols[22].symbolvalue = ident;
	lastoflength[6] = 22;

	strcpy(wordsymbols[23].symbolspelling, "PROGRAM  ");
	wordsymbols[23].symbolvalue = programsy;
	strcpy(wordsymbols[24].symbolspelling, "         ");
	wordsymbols[24].symbolvalue = ident;
	lastoflength[7] = 24;

	strcpy(wordsymbols[25].symbolspelling, "         ");
	wordsymbols[25].symbolvalue = ident;
	lastoflength[8] = 25;

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
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z': {
		k = 0;
		strcpy(spelling, "         ");

		do {
			if (k < alfalength) {
				spelling[k] = toupper(ch);
				k++;
			}
			nextch();
		} while ((ch >= '0') && (ch <= '9')
				|| (toupper(ch) >= 'A') && (toupper(ch) <= 'Z'));

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
		nextch();
		if (ch == '.') {
			symbol = thru;
			nextch();
		} else {
			symbol = period;
		}
	}
		break;
	case '<': {
		nextch();
		if (ch == '=') {
			symbol = leop;
			nextch();
		} else if (ch == '>') {
			symbol = neop;
			nextch();
		} else {
			symbol = ltop;
		}
	}
		break;
	case '>': {
		nextch();
		if (ch == '=') {
			symbol = geop;
			nextch();
		} else {
			symbol = gtop;
		}
	}
		break;
		// 1-character operator delimiter
	case '+': {
		symbol = plus;
		nextch();
	}
		break;
	case '-': {
		symbol = minus;
		nextch();
	}
		break;
	case '*': {
		symbol = times;
		nextch();
	}
		break;
	case '=': {
		symbol = eqop;
		nextch();
	}
		break;
	case '(': {
		symbol = leftparent;
		nextch();
	}
		break;
	case ')': {
		symbol = rightparent;
		nextch();
	}
		break;
	case '[': {
		symbol = leftbracket;
		nextch();
	}
		break;
	case ']': {
		symbol = rightbracket;
		nextch();
	}
		break;
	case ',': {
		symbol = comma;
		nextch();
	}
		break;
	case ';': {
		symbol = semicolon;
		nextch();
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
