#include <gb/gb.h>

#include "sprites.h"
#include "sprites.c"

#define SP_NULL 0
#define SP_SHIP 1

int n;
UBYTE key;

typedef struct {
    int x;
    int y;
} player_t;

player_t player;

void init_screen() {
    SPRITES_8x8;

    set_sprite_data(0, 2, sprites);

    // clear OAM
    for (n = 0; n < 40; n++) {
        set_sprite_tile(n, SP_NULL);
    }

    SHOW_SPRITES;
}

void init_player() {
    set_sprite_tile(0, SP_SHIP);
    player.x = 84;
    player.y = 140;
    //move_sprite(0, 84, 140);
}

void move_player() {
    key = joypad();

    if (key & J_LEFT) {
        if (player.x > 8) player.x--;
    } else if (key & J_RIGHT) {
        if (player.x < 160) player.x++;
    }

    if (key & J_UP) {
        if (player.y > 16) player.y--;
    } else if (key & J_DOWN) {
        if (player.y < 151) player.y++;
    }

    move_sprite(0, player.x, player.y);
}

void main() {
    disable_interrupts();
    DISPLAY_OFF;

    init_screen();
    init_player();

    DISPLAY_ON;
    enable_interrupts();

    while (1) {
        wait_vbl_done();
        move_player();
    }
}

/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
