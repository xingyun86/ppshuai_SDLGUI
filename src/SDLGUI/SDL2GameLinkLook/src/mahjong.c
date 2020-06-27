#include <stdlib.h>
#include <stdio.h>

#include "game.h"

int main(int argc, char **argv)
{
    struct Game *game = NULL;

    game = CALLOC(struct Game);
    if (!game) {
        fprintf(stderr, "Create game failed.\n");
        exit(EXIT_FAILURE);
    }

    if (!game_init(game)) {
        fprintf(stderr, "Initialize scene failed.\n");
        free(game);
        exit(EXIT_FAILURE);
    }

    game_mainloop(game);
    game_destroy(game);

    free(game);
    exit(EXIT_SUCCESS);
}
