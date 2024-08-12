#include "compiler.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: wave <filename.wave>\n");
		return 1;
	}

	const char *input_filename = argv[1];
	const char *output_c_filename = "output.c";

	compile_wave_to_c(input_filename, output_c_filename);
	compile_and_run(output_c_filename);

	return 0;
}