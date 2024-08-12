#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void process_line(char* line, FILE* output_file);

// Helper function to determine if a string represents an integer
int is_integer(const char* str) {
    while (*str) {
        if (!isdigit(*str) && *str != '-') {
            return 0;
        }
        str++;
    }
    return 1;
}

void handle_import(const char* module_name, FILE* output_file) {
    char wave_filename[300];
    snprintf(wave_filename, sizeof(wave_filename), "examples/%s.wave", module_name);

    // printf("Importing module: %s\n", wave_filename);  // Debug message

    FILE* import_file = fopen(wave_filename, "r");
    if (!import_file) {
        perror("Failed to open imported module");
        exit(EXIT_FAILURE);
    }

    char import_line[256];
    while (fgets(import_line, sizeof(import_line), import_file)) {
        // printf("Processing line from import: %s", import_line);  // Debug message
        process_line(import_line, output_file);
    }

    fclose(import_file);
}

void process_line(char* line, FILE* output_file) {
    // printf("Processing line: %s", line);  // Debug message

    // Trim newline characters
    line[strcspn(line, "\r\n")] = '\0';

    if (strstr(line, "fun")) {
        fprintf(output_file, "void ");
        char* start = strstr(line, "fun") + 4;
        char* end = strchr(start, '{');
        if (end) *end = '\0';
        fprintf(output_file, "%s {\n", start);
    }
    else if (strstr(line, "var")) {
        // Handle variable declarations
        char* start = strstr(line, "var") + 4;
        char* eq_pos = strchr(start, '=');
        if (eq_pos) {
            *eq_pos = '\0'; // Split the variable name and the value
            char* name = start;
            char* value = eq_pos + 1;

            // Determine variable type and format accordingly
            if (is_integer(value)) {
                fprintf(output_file, "int %s = %s;\n", name, value);
            }
            else if (value[0] == '"') {
                // Handle strings (assuming double quotes for string literals)
                fprintf(output_file, "char %s[] = %s;\n", name, value);
            }
            else {
                fprintf(output_file, "int %s = %s;\n", name, value); // Default to int if unknown
            }
        }
    }
    else if (strstr(line, "import")) {
        char module_name[256];
        sscanf_s(line, "import %s", module_name);
        handle_import(module_name, output_file);
    }
    else if (strstr(line, "if") || strstr(line, "else") || strstr(line, "while")) {
        fprintf(output_file, "%s\n", line);
    }
    else {
        fprintf(output_file, "%s\n", line);
    }
}

void compile_wave_to_c(const char* input_filename, const char* output_filename) {
    FILE* input_file = fopen(input_filename, "r");
    FILE* output_file = fopen(output_filename, "w");

    if (!input_file || !output_file) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    fprintf(output_file, "#include <stdio.h>\n\n");

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        process_line(line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void compile_and_run(const char* c_filename) {
    char command[256];
    snprintf(command, sizeof(command), "gcc %s -o wave_output && wave_output", c_filename);
    system(command);
}