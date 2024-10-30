// compiler.rs
use std::fs::File;
use std::io::{self, BufRead, Write};
use std::path::Path;
use std::process::Command;

fn is_integer(s: &str) -> bool {
    s.chars().all(|c| c.is_digit(10) || c == '-')
}

fn handle_import(module_name: &str, output_file: &mut File) -> io::Result<()> {
    let wave_filename = format!("examples/{}.wave", module_name);
    let import_file = File::open(&wave_filename)?;

    let reader = io::BufReader::new(import_file);
    for line in reader.lines() {
        let line = line?;
        process_line(&line, output_file)?;
    }

    Ok(())
}

fn process_line(line: &str, output_file: &mut File) -> io::Result<()> {
    let line = line.trim_end_matches(&['\r', '\n'] as &[_]);

    if line.contains("fun") {
        writeln!(output_file, "void {} {{", &line[4..line.find('{').unwrap_or(line.len())])?;
    } else if line.contains("var") {
        let parts: Vec<&str> = line[4..].split('=').collect();
        if parts.len() == 2 {
            let name = parts[0].trim();
            let value = parts[1].trim();

            // 여기에서 var를 삭제하고 int로만 변경합니다.
            if is_integer(value) {
                writeln!(output_file, "int {} = {};", name, value)?; // 'var'를 'int'로 수정
            } else if value.starts_with('"') {
                writeln!(output_file, "char {}[] = {};", name, value)?;
            } else {
                writeln!(output_file, "int {} = {};", name, value)?;
            }
        }
    } else if line.contains("import") {
        let module_name = line.split_whitespace().nth(1).unwrap_or("");
        handle_import(module_name, output_file)?;
    } else if line.contains("if") || line.contains("else") || line.contains("while") {
        writeln!(output_file, "{}", line)?;
    } else {
        writeln!(output_file, "{}", line)?;
    }

    Ok(())
}

pub fn compile_wave_to_c(input_filename: &str, output_filename: &str) -> io::Result<()> {
    let input_file = File::open(input_filename)?;
    let mut output_file = File::create(output_filename)?;

    writeln!(output_file, "#include <stdio.h>\n")?;

    let reader = io::BufReader::new(input_file);
    for line in reader.lines() {
        let line = line?;
        process_line(&line, &mut output_file)?;
    }

    Ok(())
}

pub fn compile_and_run(c_filename: &str) -> io::Result<()> {
    Command::new("gcc")
        .arg(c_filename)
        .arg("-o")
        .arg("wave_output")
        .status()?;

    Command::new("./wave_output").status()?;

    Ok(())
}
