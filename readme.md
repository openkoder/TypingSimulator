# Typing Simulator

## Description

**Typing Simulator** is a simple console application that emulates the Vim editor. This program is designed to simulate the process of typing text, including the ability to introduce random errors, correct them, and automatically wrap lines when the end of the screen is reached. Typing Simulator is particularly useful for recording programming screencasts, as it allows you to create realistic demonstrations of working in a text editor. The **Typing Simulator** is written in C using the ncurses library to manage the text interface.

### Key Features:
- Simulates the process of typing with adjustable speed.
- Ability to introduce random typos for added realism.
- Automatically wraps lines when the end of the screen is reached.
- Syntax highlighting for keywords and strings in C/C++ languages.
- Displays a Vim-like status line with the current cursor position.
- Supports wide characters and uses the ncurses library for terminal manipulation.

## Installation

To compile the program, you need to have the `ncurses` or `ncursesw` library installed. You can compile the program using the following command:

```bash
gcc typing_simulator.c -o typing_simulator -lncursesw
```

If you encounter issues with `ncursesw`, try using `ncurses`:

```bash
gcc typing_simulator.c -o typing_simulator -lncurses
```

## Usage

After compilation, you can run the program with the following command:

```bash
./typing_simulator
```

The program will simulate the process of typing text from the "code.c" file, highlighting keywords and strings, and displaying a Vim-like status line.

### Example Usage:

```bash
./typing_simulator example.c
```

## Configuration

You can modify the program's parameters, such as the minimum and maximum delay between typing characters, as well as the probability of errors, by changing the corresponding `#define` values in the code:

```c
#define DELAY_MIN 40000      // Minimum delay in microseconds
#define DELAY_MAX 400000     // Maximum delay in microseconds
#define ERROR_PROB 0.018     // Probability of making an error (1.8%)
```

## Applications

**Typing Simulator can be useful for:**

- Recording programming screencasts: Create a realistic simulation of live coding.
- Just for fun: Watch your computer "type" code on its own.

## Platform Support

The program is compatible with Linux and macOS. For Windows, it is recommended to use terminal emulators like WSL.

## License

This project is licensed under the BSD 2-Clause "Simplified" License. For more details, see the LICENSE file.

## Conclusion

Typing Simulator is a handy tool for creating realistic demonstrations of working in Vim, which can be useful for recording educational videos.
