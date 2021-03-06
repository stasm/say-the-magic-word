#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "game.h"
#include "input.h"
#include "render.h"

#define MIN_LETTERS  7
#define MAX_LETTERS  12

/* djb2 algorithm: http://www.cse.yorku.ca/~oz/hash.html */
static unsigned int
hash(char *str)
{
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

static bool
is_lowercase_ascii(char *s)
{
    int len = 0;

    while (*s) {
        if (122 < *s || *s < 97)
            return false;
        s++;
        len++;
    }

    return MIN_LETTERS <= len && len <= MAX_LETTERS;
}

int
main()
{
    setlocale(LC_ALL, "");

    int count = 0;
    char word[13];
    char line[80];
    FILE *words = fopen("/usr/share/dict/words", "r");

    srand(time(0));

    while (fgets(line, sizeof line, words) != NULL) {
        strtok(line, "\n");
        if (is_lowercase_ascii(line) && rand() % ++count == 0)
            strcpy(word, line);
    }

    struct game_state game;
    struct input_state input;
    struct render_ui ui;

    srand(hash(word));

    render_init(&ui);
    game_init(&game, word);

    do {
        render(&game, &ui);
        game_process(&game, &input);
    } while (!input.quit);


    render_teardown(&ui);

    printf("The word was: %s.\n", word);
}
