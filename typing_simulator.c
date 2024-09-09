// gcc typing_simulator.c -o typing_simulator -lncursesw
// gcc typing_simulator.c -o typing_simulator -lncurses

#define _XOPEN_SOURCE_EXTENDED 1

/* The path for the wide-character curses library. 

#ifndef NCURSESW_INCLUDE_H

#if defined(__APPLE__) || !defined(__linux__) || defined(__FreeBSD__)
#define NCURSESW_INCLUDE_H <curses.h>
#else
#define NCURSESW_INCLUDE_H <ncursesw/curses.h>
#endif

#endif

#include NCURSESW_INCLUDE_H 
*/

#include <curses.h>  // Library for terminal manipulation (text-based UI) in Manjaro and Arch Linux
//#include <ncursesw/curses.h>  // Library for terminal manipulation (text-based UI)
//#include <ncurses.h>  // Library for terminal manipulation (text-based UI)
#include <unistd.h>   // For usleep() function to introduce delays
#include <stdlib.h>   // For standard functions like rand(), srand(), malloc(), etc.
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>     // For time() function to seed the random number generator
#include <string.h>   // For string manipulation functions like strlen(), strcmp(), etc.
#include <ctype.h>    // For character classification functions like isalnum()


// The following lines define the minimum and maximum delay between character typing
// DELAY_MIN: Minimum delay in microseconds
// DELAY_MAX: Maximum delay in microseconds
//#define DELAY_MIN 40000      // Default minimum delay in microseconds
//#define DELAY_MAX 400000     // Default maximum delay in microseconds
#define DELAY_MIN 40000      // Minimum delay in microseconds
#define DELAY_MAX 400000     // Maximum delay in microseconds

// ERROR_PROB defines the probability of making a typing error (e.g., random character insertion)
//#define ERROR_PROB 0.018     // Default probability of making a typing error (1.8%)
#define ERROR_PROB 0.018     // Probability of making a typing error (1.8%)


// Global variables
int count_of_scroll = 0;     // Tracks the number of times the window has scrolled

int color_pair = 1;          // Stores the current color pair used for text display

int console_lines; // Number of lines in the terminal window
int console_cols; // Number of columns in the terminal window

// Function to introduce a delay in milliseconds
void ms_sleep(int ms) {
	usleep(ms * 1000);  // Convert milliseconds to microseconds and sleep
}

// Function to update the status line that shows the current cursor position
void update_status_line() {
	int y, x;
	getyx(stdscr, y, x);  // Get the current cursor position
	console_lines	 = getmaxy(stdscr);  // Get the total number of lines in the terminal
	int width = getmaxx(stdscr);  // Get the total number of columns (width of the terminal)

	// Buffer to hold the full status line
	char status[width + 1]; // +1 for the '\0' character

	// Create the status line with "-- INSERT --" at the start
	snprintf(status, sizeof(status), "-- INSERT --");

	// Calculate the position to start printing the cursor coordinates
	int coord_position = width - 18;  // 18 characters from the right edge

	// Fill the space between "-- INSERT --" and the coordinates with spaces
	memset(status + 12, ' ', coord_position - 12);

	// Convert coordinates to string
	char coord_str[15];
	snprintf(coord_str, sizeof(coord_str), "%d,%d", y + 1 + count_of_scroll, x + 1);

	// Append the coordinates at the calculated position
	strncpy(status + coord_position, coord_str, strlen(coord_str));

	// Fill the remaining space after the coordinates with spaces
	int coord_length = strlen(coord_str);
	memset(status + coord_position + coord_length, ' ', width - (coord_position + coord_length));

	// Null-terminate the status string
	status[width] = '\0';


	// Move the cursor to the bottom of the screen
	move(console_lines	 - 1, 0);

	// Clear the status line to remove remnants from previous updates
	//clrtoeol();

	scrollok(stdscr, FALSE);  // Disable scrolling for the window
	// Display the status line with the current color pair
	attron(COLOR_PAIR(2));
	//attron(A_REVERSE);
	printw("%s", status);
	//attroff(A_REVERSE);
	attroff(COLOR_PAIR(2));

	// Return the cursor to its original position
	move(y, x);
	refresh();  // Refresh the screen to reflect changes
}


// Function to delete the last character typed
void delete_last_char() {
	if (getcurx(stdscr) > 0) {  // Ensure the cursor is not at the beginning of the line
		move(getcury(stdscr), getcurx(stdscr) - 1);  // Move cursor one position left
		addch(' ');  // Replace the character with a space (effectively deleting it)
		move(getcury(stdscr), getcurx(stdscr) - 1);  // Move cursor back to the deleted position
		refresh();

		// Introduce a random delay to simulate realistic typing speed
		usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);

		// Update the status line after deleting the character
		update_status_line();
	}
}

// Function to introduce a random typing error by inserting a random character
void type_with_errors() {
	// Character set including letters, digits, and special symbols
	const char char_set[] = 
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		"`~!@#$%^&*()-=_+[]\\{}|;':\",./<>? ";
	int num_of_chars = sizeof(char_set) - 1;  // Total number of characters in the set
	char randomChar = char_set[rand() % num_of_chars];  // Pick a random character

	// Display the random character
	addch(randomChar);
	refresh();
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);

	// Delete the random character to simulate a typing error correction
	delete_last_char();
	attron(COLOR_PAIR(color_pair));  // Restore the color pair after correction
	refresh();
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);
}

void type_char(wchar_t c) {
	int y, x;
	getyx(stdscr, y, x); // Get the current cursor position (y, x)

	// Check if the cursor is on the second to last line and the character to be typed is a newline
	if (y == LINES - 2 && c == '\n') {  // LINES - 2 ensures the status line is preserved
		// Clear the last line to make space for the status line
		move(console_lines	 - 1, 0);
		clrtoeol();  // Clear from the cursor to the end of the line
		
		// Move the cursor to the second to last line
		move(console_lines	 - 2 - 1, x);

		//refresh();  // Refresh the screen to apply the changes
		
		scrollok(stdscr, TRUE);  // Enable scrolling for the window
		wscrl(stdscr, 1);  // Scroll the window content up by one line
		refresh();  // Refresh the screen to apply the scrolling

		// Increment the scroll count to keep track of how many times the window has scrolled
		count_of_scroll++;
	}
	// Automatically wrap the line if the character will exceed the screen width
	if (x >= console_cols - 1 && c != '\n') {
		// Move to the next line
		x = 0;
		y++;
		
		// Decrement the scroll count to keep track of how many times the window has scrolled
		count_of_scroll--;
		
		if (y >= LINES - 1) {  // If we reach the last line, scroll up
			// Clear the last line to make space for the status line
			move(console_lines	 - 1, 0);
			clrtoeol();  // Clear from the cursor to the end of the line
			
			// Move the cursor to the second to last line
			move(console_lines	 - 1 - 1, x);
			
			scrollok(stdscr, TRUE);  // Enable scrolling for the window
			wscrl(stdscr, 1);  // Scroll the window content up by one line
			refresh();  // Refresh the screen to apply the scrolling
			
			// Increment the scroll count to keep track of how many times the window has scrolled
			count_of_scroll++;
		}
	}

	// Randomly decide whether to introduce a typing error based on ERROR_PROB
	if ((double)rand() / RAND_MAX < ERROR_PROB) {
		type_with_errors();  // Introduce a typing error
	}


	cchar_t wch;
	wchar_t wstr[2] = {c, L'\0'};
	setcchar(&wch, wstr, A_NORMAL, 0, NULL);
	add_wch(&wch);  // Type the actual character
	refresh();  // Refresh the screen to display the typed character
	update_status_line();  // Update the status line with the current cursor position
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);  // Introduce a delay to simulate realistic typing speed
}


void print_highlighted(const wchar_t* code) {
	// List of C/C++ keywords to be highlighted
	const wchar_t* keywords[] = {L"int", L"bool", L"vector", L"return", L"if", L"else", L"while", L"for", L"void"};
	int num_keywords = sizeof(keywords) / sizeof(keywords[0]);  // Calculate the number of keywords

	while (*code) {
		int is_keyword = 0;  // Flag to check if the current word is a keyword
		
		for (int i = 0; i < num_keywords; i++) {
			size_t len = wcslen(keywords[i]);  // Get the length of the current keyword
			// Check if the current segment of code matches a keyword and is followed by a non-alphanumeric character
			if (wcsncmp(code, keywords[i], len) == 0 && !iswalnum(code[len])) {
				// If it's a keyword, highlight it
				for (size_t j = 0; j < len; j++) {
					color_pair = 2;
					attron(COLOR_PAIR(2));  // Set color for keywords
					type_char(*code++);  // Type the keyword character by character
					attroff(COLOR_PAIR(2));  // Turn off keyword color attribute
				}
				is_keyword = 1;  // Mark that we found a keyword
				break;  // Break out of the loop since the keyword has been processed
			}
		}
		
		if (!is_keyword) {  // If the current segment is not a keyword
			if (*code == L'"') {  // Check if it's the start of a string literal
				do {
					color_pair = 3;
					attron(COLOR_PAIR(3));  // Set color for string literals
					type_char(*code++);  // Type the string character by character
					attroff(COLOR_PAIR(3));  // Turn off string color attribute
				} while (*code && *code != L'"');  // Continue until the end of the string literal
				color_pair = 3;
				attron(COLOR_PAIR(3));  // Set color for the closing quote of the string
				type_char(*code++);  // Type the closing quote
				attroff(COLOR_PAIR(3));  // Turn off string color attribute
			} else {  // If it's not a keyword or string literal, treat it as regular code
				color_pair = 1;
				attron(COLOR_PAIR(1));  // Set color for regular code
				type_char(*code++);  // Type the character
				attroff(COLOR_PAIR(1));  // Turn off regular code color attribute
			}
		}
	}
}


void read_code_from_file(const char* filename, wchar_t** buffer) {
	// Open the file in read mode
	FILE* file = fopen(filename, "r");
	if (!file) {
		// If the file fails to open, print an error message and terminate the program
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	// Move the file pointer to the end of the file to determine the file size
	fseek(file, 0, SEEK_END);
	long length = ftell(file);  // Get the current file pointer position, which is the file size
	fseek(file, 0, SEEK_SET);  // Reset the file pointer back to the beginning of the file


	// Allocate memory to hold the contents of the file plus one extra byte for the null-terminator
	char* temp_buffer = malloc(length + 1);
	if (!temp_buffer) {
		// If memory allocation fails, print an error message and terminate the program
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	// Read the entire file into the buffer
	fread(temp_buffer, 1, length, file);
	temp_buffer[length] = '\0';  // Add a null-terminator at the end of the buffer to make it a valid C string

	// Close the file
	fclose(file);


	// Конвертируем мультибайтовую строку в широкую строку
	size_t wide_length = mbstowcs(NULL, temp_buffer, 0) + 1;
	*buffer = malloc(wide_length * sizeof(wchar_t));
	if (!*buffer) {
		perror("Failed to allocate memory for wide string");
		exit(EXIT_FAILURE);
	}

	mbstowcs(*buffer, temp_buffer, wide_length);

	free(temp_buffer);
}


int main() {
	// Seed the random number generator using the current time
	srand(time(NULL));

	// Initialize ncurses
	setlocale(LC_ALL, "");
	initscr();          // Start curses mode
	start_color();      // Enable color functionality in the terminal

	// Set default background color (default terminal background)
	use_default_colors();

	// Set the terminal to "cbreak" mode, where input is immediately available to the program
	cbreak();
	noecho();           // Disable echoing of input characters
	curs_set(1);        // Make the cursor visible

	// Define color pairs for different types of text
	init_pair(1, COLOR_GREEN, -1);   // Color pair 1: Green text on transparent background (regular code text)
	init_pair(2, COLOR_WHITE, -1);   // Color pair 2: White text on transparent background (keywords)
	init_pair(3, COLOR_YELLOW, -1);  // Color pair 3: Yellow text on transparent background (string literals)

	// Clear the terminal screen
	clear();

	console_lines	= getmaxy(stdscr); // Get the total number of lines in the terminal window
	console_cols	= getmaxx(stdscr); // Get the total number of columns in the terminal window

	wchar_t* code = NULL;  // Pointer to hold the code read from the file
	read_code_from_file("code.c", &code);  // Read the code from "code.c" into the buffer

	print_highlighted(code);  // Print the code with syntax highlighting

	free(code);  // Free the allocated memory for the code

	// Update and display the status line with the current cursor position
	update_status_line();  // Call the function to display the status line

	// Wait for a key press before exiting
	getch();

	// End ncurses mode and restore the terminal to its normal state
	curs_set(0);  // Hide the cursor
	endwin();     // End curses mode

	return 0;
}
