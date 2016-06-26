#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "input.h"

bool equals(struct grid_pos a, struct grid_pos b)
{
    return a.x == b.x && a.y == b.y;
}

struct letter *get_letter(struct game_state *game, struct grid_pos pos)
{
    for (size_t i = 0; i < game->num_letters; i++)
        if (equals(game->letters[i].pos, pos))
            return &game->letters[i];

    return NULL;
}

void capture_letter(struct letter *letter)
{
    letter->captured = true;
}

void move_player(struct game_state *game, enum input_dir dir)
{
    struct grid_pos new_pos = game->player.pos;

    switch (dir) {
        case DIRECTION_NW:
        case DIRECTION_N:
        case DIRECTION_NE:
            new_pos.y--;
        default:
            break;
    }

    switch (dir) {
        case DIRECTION_NE:
        case DIRECTION_E:
        case DIRECTION_SE:
            new_pos.x++;
        default:
            break;
    }

    switch (dir) {
        case DIRECTION_SW:
        case DIRECTION_S:
        case DIRECTION_SE:
            new_pos.y++;
        default:
            break;
    }

    switch (dir) {
        case DIRECTION_NW:
        case DIRECTION_W:
        case DIRECTION_SW:
            new_pos.x--;
        default:
            break;
    }

    struct letter *letter = get_letter(game, new_pos);

    if (letter != NULL)
        capture_letter(letter);

    if (game->map[new_pos.y][new_pos.x] == TILE_EMPTY)
        game->player.pos = new_pos;
}

void game_process(struct game_state *game, struct input_state *input)
{
    input_get(input);

    if (input->torch)
        game->player.torch = !game->player.torch;

    move_player(game, input->dir);
}


void game_init(struct game_state *game, char *magic_word)
{
    dungeon_init(game);

    game->player = (struct player) {
        .pos   = dungeon_rand_pos(game),
        .torch = false
    };

    game->magic_word = magic_word;
    game->num_letters = strlen(magic_word);
    game->letters = malloc(game->num_letters * sizeof(struct letter));

    if (game->letters != NULL)
        for (size_t i = 0; i < game->num_letters; i++)
            game->letters[i] = (struct letter ) {
                game->magic_word[i], dungeon_rand_pos(game), false
            };
}
