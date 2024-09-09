// gcc typing_simulator.c -o typing_simulator -lncurses

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

//#define DELAY_MIN 30000	// Минимум задержки в микросекундах
//#define DELAY_MAX 300000	// Максимум задержки в микросекундах
#define DELAY_MIN 40000		// Минимум задержки в микросекундах
#define DELAY_MAX 400000	// Максимум задержки в микросекундах
//#define ERROR_PROB 0.05	// Вероятность ошибки
#define ERROR_PROB 0.020	// Вероятность ошибки

int count_of_scroll = 0;	// Количество прокрутки окна
int color_pair = 1;			// Переменная для сохранения текущего цвета печати в консоли

void ms_sleep(int ms) {
    usleep(ms * 1000);
}

void update_status_line() {
	int y, x;
	getyx(stdscr, y, x);
	int current_lines = getmaxy(stdscr);

	// Создание строки состояния
	char status[140];
	snprintf(status, sizeof(status), "-- INSERT --                                                                                                                    %d,%d", y + 1 + count_of_scroll, x + 1);
	//mvprintw(LINES - 1, 0, "-- INSERT --                                                               %d,%d", y, x);

	// Перемещение курсора в самый низ экрана
	//move(LINES - 1, 0);
	move(current_lines - 1, 0);

	// Очистка строки состояния (чтобы не было остатков от предыдущих обновлений)
	//getch();  // test: Ждем нажатия любой клавиши для проверки
	//clrtoeol();


	// Отображение строки состояния
	//attron(A_REVERSE);  // Обратные цвета для выделения строки состояния
	attron(COLOR_PAIR(1));
	printw("%s", status);
	attroff(COLOR_PAIR(1));
	//attroff(A_REVERSE);

	// Возвращение курсора на его исходную позицию
	move(y, x);
	// Отобразить все изменения на экране
	refresh();
}


void delete_last_char() {
	if(getcurx(stdscr) > 0) {
		move(getcury(stdscr), getcurx(stdscr) - 1);
		addch(' ');
		move(getcury(stdscr), getcurx(stdscr) - 1);
		refresh();

		usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);

		update_status_line();  // Обновляем строку состояния после вывода символа
	}
}

// Ошибка: печатаем случайный символ
void type_with_errors () {
	// Генерируем случайный символ из расширенного набора
	const char char_set[] = 
		"abcdefghijklmnopqrstuvwxyz" // строчные буквы
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" // заглавные буквы
		"0123456789"                 // цифры
		"`~!@#$%^&*()-=_+[]\\{}|;':\",./<>? ";  // спецсимволы
	int num_of_chars = sizeof(char_set) - 1; // -1, т.к. последний символ '\0'
	char randomChar = char_set[rand() % num_of_chars]; 

	
	//attron(COLOR_PAIR(1));  // Цвет для обычных строк
	addch(randomChar);
	//attroff(COLOR_PAIR(1));  // Сброс цвета
	refresh();
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);
	
	// Удаляем его
	delete_last_char();
	attron(COLOR_PAIR(color_pair));  // Цвет для обычных строк
	refresh();
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);
}

void type_char(char c) {
	int y, x;
	getyx(stdscr, y, x); // Получаем текущие координаты курсора
	int current_lines = getmaxy(stdscr);

	// Проверяем, достигли ли мы предпоследней строки и выводим ли символ новой строки
	if (y == LINES - 2 && c == '\n') {  // -2, чтобы не затереть строку состояния
		// Очистка последней строки (где выводится строка состояния)
		move(current_lines - 1, 0);
		clrtoeol();
		
		// Перемещаем курсор на предпоследнюю строку
		move(current_lines - 2 - 1, x);

		refresh();
		
		scrollok(stdscr, TRUE);         // Включаем прокрутку
		wscrl(stdscr, 1);               // Сдвигаем содержимое окна вверх на одну строку
		refresh();
        
		// Перемещаем курсор на предпоследнюю строку
		//move(LINES - 2 - 1, 0);
		//move(current_lines - 2 - 1, x);
		
		// Увеличиваем счётчик прокрутки окна для вывода информации 
		// в строке состояния
		count_of_scroll++;
	}

	if ((double)rand() / RAND_MAX < ERROR_PROB) {
		type_with_errors();
	}

	addch(c);
	// Отобразить все изменения на экране
	refresh();
	update_status_line();  // Обновляем строку состояния после вывода символа
	usleep(rand() % (DELAY_MAX - DELAY_MIN + 1) + DELAY_MIN);
}


void print_highlighted(const char* code) {
	const char* keywords[] = {"int", "bool", "vector", "return", "if", "else", "while", "for", "void"};
	int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

	while (*code) {
		int is_keyword = 0;
		for (int i = 0; i < num_keywords; i++) {
			size_t len = strlen(keywords[i]);
			if (strncmp(code, keywords[i], len) == 0 && !isalnum(code[len])) {
				for (size_t j = 0; j < len; j++) {
					color_pair = 2;
					attron(COLOR_PAIR(2));  // Цвет для ключевых слов
					type_char(*code++);
					attroff(COLOR_PAIR(2));  // Сброс цвета
				}
				is_keyword = 1;
				break;
			}
		}
		if (!is_keyword) {
			if (*code == '"') {
				do {
					color_pair = 3;
					attron(COLOR_PAIR(3));  // Цвет для переменных с типом строк
					type_char(*code++);
					attroff(COLOR_PAIR(3));  // Сброс цвета
				} while (*code && *code != '"');
				color_pair = 3;
				attron(COLOR_PAIR(3));  // Цвет для переменных с типом строк
				type_char(*code++);
				attroff(COLOR_PAIR(3));  // Сброс цвета
			} else {
				color_pair = 1;
				attron(COLOR_PAIR(1));  // Цвет для обычных строк
				type_char(*code++);
				attroff(COLOR_PAIR(1));  // Сброс цвета
			}
		}
	}
}

void read_code_from_file(const char* filename, char** buffer) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Не удалось открыть файл");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = malloc(length + 1);
    if (!*buffer) {
        perror("Не удалось выделить память");
        exit(EXIT_FAILURE);
    }

    fread(*buffer, 1, length, file);
    (*buffer)[length] = '\0';

    fclose(file);
}


int main() {
    srand(time(NULL));

    // Инициализация ncurses
    initscr();
    start_color();

    // Установка фона по умолчанию
    use_default_colors();

    cbreak();
    noecho();
    curs_set(1); // Включить отображение курсора

    // Определение цветовых пар
    init_pair(1, COLOR_GREEN, -1); // Цвет для обычных строк (прозрачный фон)
    init_pair(2, COLOR_WHITE, -1);  // Цвет для ключевых слов (прозрачный фон)
    init_pair(3, COLOR_YELLOW, -1); // Цвет для переменных с типом строк (прозрачный фон)

    // Очистка экрана
    clear();

    char* code = NULL;
    read_code_from_file("code.c", &code);

    print_highlighted(code);

    free(code);

    // Обновление и отображение позиции курсора
    update_status_line(); // Вызов функции для отображения строки состояния

    // Ожидание ввода перед выходом
    getch();

    // Завершение работы с ncurses
    curs_set(0); // Скрыть курсор
    endwin();

    return 0;
}
