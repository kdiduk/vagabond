#include <arch/zx.h>
#include <stdbool.h>
#include "display.h"
#include "forest.h"
#include "level.h"
#include "player.h"
#include "viewport.h"


#define LEVEL_WIDTH (32)
#define LEVEL_HEIGHT (20)

static bool screen_invalid;
static uint8_t xleft;
static uint8_t ytop;

static void center_screen(void);
static void redraw_level(void);
static bool can_move(int8_t dx, int8_t dy);
static void draw_tile(uint8_t x, uint8_t y);

void level_init(void)
{
        viewport_init();
        forest_init();
        player_setpos(forest_spawnx(), forest_spawny());
        center_screen();
}

void level_draw(void)
{
        if (screen_invalid) {
                redraw_level();
                screen_invalid = false;
        }

        viewport_render();
}

void level_player_move(uint8_t direction)
{
        int8_t dx = 0;
        int8_t dy = 0;
        switch (direction) {
        case 'y':
                dx--;
                dy--;
                break;
        case 'u':
                dx++;
                dy--;
                break;
        case 'h':
                dx--;
                break;
        case 'k':
                dy--;
                break;
        case 'j':
                dy++;
                break;
        case 'l':
                dx++;
                break;
        case 'b':
                dx--;
                dy++;
                break;
        case 'n':
                dx++;
                dy++;
                break;
        }

        if (can_move(dx, dy)) {
                draw_tile(player_posx(), player_posy());
                player_moveby(dx, dy);
                viewport_at(player_posx()-xleft, player_posy()-ytop, '@', INK_WHITE);
                if (player_posx() == xleft
                    || player_posx() == xleft + LEVEL_WIDTH-1
                    || player_posy() == ytop
                    || player_posy() == ytop + LEVEL_HEIGHT-1) {
                        center_screen();
                }
        }
}

static void center_screen(void)
{
        if (player_posx() < LEVEL_WIDTH/2) {
                xleft = 0;
        }
        else if (player_posx() < forest_width() - LEVEL_WIDTH/2) {
                xleft = player_posx() - LEVEL_WIDTH/2;
        }
        else {
                xleft = forest_width() - LEVEL_WIDTH;
        }

        if (player_posy() < LEVEL_HEIGHT/2) {
                ytop = 0;
        }
        else if (player_posy() < forest_height() - LEVEL_HEIGHT/2) {
                ytop = player_posy() - LEVEL_HEIGHT/2;
        }
        else {
                ytop = forest_height() - LEVEL_HEIGHT;
        }

        screen_invalid = true;
}

static void redraw_level(void)
{
        zx_cls(PAPER_BLACK);

        uint8_t y1 = ytop;
        uint8_t y2 = y1 + LEVEL_HEIGHT;
        uint8_t x1 = xleft;
        uint8_t x2 = x1 + LEVEL_WIDTH;

        for (uint8_t y = y1; y < y2; ++y) {
                for (uint8_t x = x1; x < x2; ++x) {
                        draw_tile(x, y);
                }
        }

        viewport_at(player_posx()-xleft, player_posy()-ytop, '@', INK_WHITE);
}

static bool can_move(int8_t dx, int8_t dy)
{
        if (dx < 0 && player_posx() == 0) {
                return false;
        }
        if (dx > 0 && player_posx() == forest_width() - 1) {
                return false;
        }

        if (dy < 0 && player_posy() == 0) {
                return false;
        }
        if (dy > 0 && player_posy() == forest_height() - 1) {
                return false;
        }

        switch (forest_tile(player_posx() + dx, player_posy() + dy)) {
        case tile_tree:
                return false;
        case tile_ground:
                return true;
        }

        return false;
}

static void draw_tile(uint8_t x, uint8_t y)
{
        switch (forest_tile(x, y)) {
        case tile_tree:
                viewport_at(x-xleft, y-ytop, '*', INK_GREEN);
                break;
        case tile_ground:
                viewport_at(x-xleft, y-ytop, '.', INK_WHITE);
                break;
        default:
                viewport_at(x-xleft, y-ytop, '?', INK_RED);
                break;
        }

}

/* EOF */
