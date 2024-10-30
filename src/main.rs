// main.rs
use std::env;
use std::process;

mod compiler;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: wave <filename.wave>");
        process::exit(1);
    }

    let input_filename = &args[1];
    let output_c_filename = "output.c";

    if let Err(e) = compiler::compile_wave_to_c(input_filename, output_c_filename) {
        eprintln!("Error compiling Wave file: {}", e);
        process::exit(1);
    }

    if let Err(e) = compiler::compile_and_run(output_c_filename) {
        eprintln!("Error running compiled program: {}", e);
        process::exit(1);
    }
}
