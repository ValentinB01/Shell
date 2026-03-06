# MyShell - Custom C Shell

A custom shell written in C, designed to emulate the behavior of a classic Linux/Unix terminal (such as bash). The project provides an interactive environment for executing external commands and includes support for advanced features like redirections, pipelines, background processes, aliases, and environment variables.

## 📋 Overview

This program acts as a command-line interpreter that handles user input, parses commands, and manages process execution using system calls like `fork`, `execvp`, and `pipe`. It aims to provide a robust and user-friendly terminal experience.

## 🚀 Getting Started

### Prerequisites

- Unix-like operating system (Linux, macOS, etc.)
- GCC (build-essential)
- GNU Readline library

### Installation

On Debian/Ubuntu-based systems, install the required dependencies with:

```bash
sudo apt-get update
sudo apt-get install build-essential libreadline-dev
```

## ⚙️ Compilation and Execution
The project uses a Makefile to automate the compilation process.

Clone the repository and navigate to the project folder.

Compile the code and start the shell using the following command:

```bash
make
./myshell
```

To clean the compiled object files and the executable, use:

```bash
make clean
```

## 💻 Usage & Examples
Here are a few examples of what you can test directly in MyShell:

### Using Pipelines and Redirections:

```bash
MyShell:/home/user$ ls -la | grep ".c" > c_files.txt
```

### Background Processes:

```bash
MyShell:/home/user$ sleep 10 &
[background] PID 12345
MyShell:/home/user$
```

### Creating Aliases and Environment Variables:

```bash
MyShell:/home/user$ export USERNAME=User
MyShell:/home/user$ alias hello='echo Hello, $USERNAME!'
MyShell:/home/user$ hello
Hello, User!
```

### Fast Navigation:

```bash
MyShell:/home/user$ cd /tmp
MyShell:/tmp$ cd -
/home/user
```

🔍 Features
✅ Executing Programs: Runs any system program found in $PATH (e.g., ls, grep, cat).

✅ Pipelines (|): Allows chaining multiple commands, redirecting the output of one to the input of the next.

✅ I/O Redirections (<, >, >>): Redirects standard input and output from and to files.

✅ Logical Operators (&&, ||): Conditional execution of commands based on the return code (exit status) of the previous command.

✅ Background Processes (&): Runs commands in the background without blocking the terminal prompt. Automatic management of "zombie" processes is ensured by handling the SIGCHLD signal.

✅ Environment Variables: Sets variables with export NAME=Value and uses them in commands via expansion (e.g., echo $NAME).

✅ Aliases: Shortens long commands (e.g., alias ll='ls -la'). Supports built-in commands for alias and unalias.

✅ Advanced Navigation: The cd command allows changing directories, including returning to the previous directory using cd -.

✅ History & Autocompletion: Integrates GNU Readline to provide TAB autocompletion and command history navigation using directional arrows.

✅ Signal Handling: Responds correctly to interruptions: prevents accidental shell closure on CTRL+C and ignores CTRL+Z for the parent process.

## 👥 Authors
Created by Buse Valentin-Cristian and Cuntan Paul.
