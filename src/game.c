#include <arch/zx.h>
#include <input.h>
#include <stdint.h>
#include "game.h"
#include "level.h"
#include "player.h"


uint8_t inkey;

void game_init()
{
    zx_cls(PAPER_BLACK);
}

void game_run()
{
    level_init();
    while (1) {
	in_wait_key();
	inkey = in_inkey();
	switch (inkey) {
	case 'y':
	case 'u':
	case 'h':
	case 'j':
	case 'k':
	case 'l':
	case 'b':
	case 'n':
	    level_player_move(inkey);
	    break;
	}
	level_draw();
	in_wait_nokey();
    }
}