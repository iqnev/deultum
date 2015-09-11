#include "compiler.h"

char def_filename[] = "lex_test.prg";

int main(int argc, char *argv[]) {
	char *filename;

	argc > 1 ? filename = argv[1] : filename = def_filename;

	ofstream fout;
	fout.open("lex_out.txt");

	scan comp(filename);

	fout << "SYMBOL SCANNED:" << endl << endl;

	do {
		switch (comp.symbol) {
		case ident:
			fout << "IDENTIFIER " << comp.spelling << endl;
			break;
		case intconst:
			fout << "INTEGER" << comp.constant << endl;
			break;
		case charconst:
			fout << "CHARACTER " << static_cast<char>(comp.constant) << endl;
			break;
		case notop:
			fout << "NOT" << endl;
			break;
		case andop:
			fout << "AND" << endl;
			break;
		case orop:
			fout << "OR" << endl;
			break;
		case times:
			fout << "*" << endl;
			break;
		case divop:
			fout << "DIV" << endl;
			break;
		case plus:
			fout << "+" << endl;
			break;
		case minus:
			fout << "-" << endl;
			break;
		case ltop:
			fout << "<" << endl;
			break;
		case leop:
			fout << "<=" << endl;
			break;
		case geop:
			fout << ">=" << endl;
			break;
		case gtop:
			fout << ">" << endl;
			break;
		case neop:
			fout << "<>" << endl;
			break;
		case eqop:
			fout << "=" << endl;
			break;
		case rightparent:
			fout << ")" << endl;
			break;
		case leftparent:
			fout << "(" << endl;
			break;
		case rightbracket:
			fout << "]" << endl;
			break;
		case leftbracket:
			fout << "[" << endl;
			break;
		case comma:
			fout << "," << endl;
			break;
		case semicolon:
			fout << ";" << endl;
			break;
		case period:
			fout << "." << endl;
			break;
		case colon:
			fout << ":" << endl;
			break;
		case becomes:
			fout << ":=" << endl;
			break;
		case thru:
			fout << ".." << endl;
			break;
		case programsy:
			fout << "PROGRAM" << endl;
			break;
		case varsy:
			fout << "VAR" << endl;
			break;
		case procsy:
			fout << "PROCEDURE" << endl;
			break;
		case arraysy:
			fout << "ARRAY" << endl;
			break;
		case ofsy:
			fout << "OF" << endl;
			break;
		case beginsy:
			fout << "BEGIN" << endl;
			break;
		case endsy:
			fout << "END" << endl;
			break;
		case ifsy:
			fout << "IF" << endl;
			break;
		case thensy:
			fout << "THEN" << endl;
			break;
		case elsesy:
			fout << "ELSE" << endl;
			break;
		case whilesy:
			fout << "WHILE" << endl;
			break;
		case dosy:
			fout << "DO" << endl;
			break;
		case readsy:
			fout << "READ" << endl;
			break;
		case writesy:
			fout << "WRITE" << endl;
			break;
		case othersy:
			comp.error(10, comp.symbolposition);
			break;
		} // switch

		comp.nextsymbol();
	} while (comp.symbol != period);
	cin.get();

	fout << "." << endl;
	fout.close();

	return 0;
}
