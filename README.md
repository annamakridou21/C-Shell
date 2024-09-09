## Table of Contents

- [C-Shell](#c-shell)
  - [Features](#features)
  - [Implementation Details](#implementation-details)
- [Requirements](#requirements)
- [Run Command](#run-command)
- [Usage](#usage)
- [Examples](#examples)

# C-Shell

This project is a simple shell implemented in C. It mimics the behavior of a typical Unix shell, supporting various features like executing simple commands, handling multiple commands separated by `;`, input/output redirection, and piping. The shell provides a prompt where users can enter commands, and it will execute them similarly to how a regular shell would.

## Features

- **Command Execution:**
  - Executes simple commands (e.g., `ls`, `pwd`).
  - Supports the `cd` command for changing directories.
  - Allows multiple commands separated by `;`.
  - Supports input redirection with `<`.
  - Supports output redirection with `>` (overwrite) and `>>` (append).
- **Error Handling:** Displays appropriate error messages for invalid commands, failed forks, and unsuccessful `execvp` calls.

## Implementation Details

- **Prompt Function (`type_prompt`)**: Constructs and displays the shell prompt.
- **Command Parsing (`check_symbol`)**: Identifies the type of command based on special symbols (`;`, `|`, `<`, `>`, `>>`).
- **Main Loop**:
  - Continuously displays the prompt and waits for user input.
  - Handles different types of commands based on user input.
  - Utilizes `fork` to create child processes for executing commands.
  - Implements input/output redirection using `dup2` and file descriptors.
  - Supports command piping by creating multiple processes and connecting them with a pipe.
  - Allows the use of `cd` to change the current working directory.

## Requirements

- **Operating System**: Linux or macOS (for POSIX compliance).
- **Compiler**: GCC or any other C compiler that supports POSIX.
- **Make**: For building the project using the `Makefile`.

## Run Command

To compile and execute the shell, use the following commands:

```bash
make
```

The `make` command will compile the source code into an executable named `c_shell`.

## Usage

Once the shell is running, you can use it to execute various commands as you would in a regular Unix shell:

- **Simple Commands**: Execute any standard Unix command like `ls`, `pwd`, etc.
- **Change Directory**: Use `cd <directory>` to change the current working directory.
- **Multiple Commands**: Use `;` to separate multiple commands in a single line.
- **Input Redirection**: Use `<` to redirect input from a file (e.g., `command < input.txt`).
- **Output Redirection**: Use `>` to overwrite or `>>` to append output to a file (e.g., `command > output.txt` or `command >> output.txt`).


## Examples

- **Simple Command**: `ls -l`
- **Multiple Commands**: `pwd; ls -l; echo "Hello, World!"`
- **Change Directory**: `cd /home/user`
- **Input Redirection**: `sort < unsorted.txt`
- **Output Redirection**: `ls -l > files.txt`
- **Append Output**: `echo "New line" >> files.txt`