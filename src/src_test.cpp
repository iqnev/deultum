#include "compiler.h"

char	def_filename[] = "srctest.prg";

int main( int argc, char *argv[] ) {
	char	*filename;

	argc > 1 ? filename = argv[1] : filename = def_filename;

	source			src( filename );
	textposition	lastquery;

	do {
		if ( src.ch == '*' ) {
			src.error( 1, src.positionnow );
		} else {
			if ( src.ch == '?' ) {
				lastquery = src.positionnow;
			} else {
				if ( src.ch == '!' ) {
					src.error( 2, lastquery );
				}
			}
		}

		src.nextch();
	} while ( src.ch != 0 );


	getchar();

	return 0;
}