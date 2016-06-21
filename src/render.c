#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "game.h"
#include "render.h"

void render_init(void)
{
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
}

void render_teardown(void)
{
    endwin();
}

static char char_for_tile(Tile tile)
{
    switch (tile) {
        case TILE_WALL:
            return '#';
        case TILE_EMPTY:
            return '.';
        case TILE_UNKNOWN:
        default:
            return '?';
    }
}

static bool is_visible(Position pos, int x, int y)
{
    int dx = pos.x - x;
    int dy = pos.y - y;
    return dx * dx + dy * dy < 25;
}

void render(GameState *game)
{
    clear();

    for (int y = 0; y < SIZEY; y++) {
        for (int x = 0; x < SIZEX; x++) {
            if (is_visible(game->player.pos, x, y))
                mvaddch(y, x, char_for_tile(game->map[y][x]));
        }
    }

    for (size_t i = 0; i < game->num_letters; i++) {
        mvaddch(
                game->letters[i].pos.y,
                game->letters[i].pos.x,
                game->letters[i].val
               );
    }

    mvaddch(game->player.pos.y, game->player.pos.x, '@');

    refresh();
}
