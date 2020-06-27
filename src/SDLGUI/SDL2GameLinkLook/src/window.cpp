#include "assets.h"
#include "util.h"
#include "window.h"

#define handle_init_error(msg) \
        ERROR_PRINT("%s, SDL_Error: %s\n", msg, SDL_GetError()); \
        window_destroy(window); \
        return false

/* Set the icon of the window */ 
static bool set_window_icon(struct Window *window)
{
        SDL_Surface *surface = NULL;
        const char *logo_file = JOIN_IMAGE("logo.png");

        surface = load_surface(logo_file);
        if (!surface) {
                return false;
        }

        SDL_SetWindowIcon(window->window, surface);

        /* The surface is no longer required */
        DESTROY_SURFACE(surface);

        return true;
}

/* Set the cursor of the window */
static bool set_window_cursor(struct Window *window)
{
        SDL_Surface *surface = NULL;
        const char *cursor_file = JOIN_IMAGE("cursor.cur");

        surface = load_surface(cursor_file);
        if (!surface) {
                return false;
        }

        window->cursor = SDL_CreateColorCursor(surface, 0, 0);
        if (!window->cursor) {
                ERROR_PRINT("Unable to create cursor, SDL_Error: %s\n", SDL_GetError());
                DESTROY_SURFACE(surface);
                return false;
        }

        SDL_SetCursor(window->cursor);

        DESTROY_SURFACE(surface);
        return true;
}

bool window_init(struct Window *window)
{
        if (!window) {
                return false;
        }
        
        /* Create the window */
        window->window = SDL_CreateWindow("LinkLook", WINDOW_X, WINDOW_Y,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window->window) {
                handle_init_error("Window could not be created");
        }

        /* Set icon */
        if (!set_window_icon(window)) {
                handle_init_error("Window icon could not be set");
        }

        /* Set cursor */
        if (!set_window_cursor(window)) {
                handle_init_error("Window cursor could not be set");
        }

        /* Create renderer for the window */
        window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
        if (!window->renderer) {
                handle_init_error("Renderer cloud not be created");
        }

        return true;
}

void window_destroy(struct Window *window)
{
        if (!window) {
                return;
        }
        
        DESTROY_CURSOR(window->cursor);
        DESTROY_RENDERER(window->renderer);
        DESTROY_WINDOW(window->window);
}
