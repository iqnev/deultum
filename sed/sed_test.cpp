#include "compiler.h"

char	def_filename[] = "sed_test.prg";

int main( int argc, char *argv[] ) {
	char	*filename;

	argc > 1	? filename = argv[1]
				: filename = def_filename;
	
	syntax	comp( filename );

	comp.programme( );

	return 0;
}