#ifndef COMPILER_H
#define COMPILER_H

void compile_wave_to_c(const char *input_filename, const char *output_filename);
void compile_and_run(const char *c_filename);

#endif // COMPILER_H