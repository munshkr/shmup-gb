#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <gb/console.h>

#include "sprites.h"
#include "sprites.c"

#define SP_NULL     0
#define SP_SHIP     1
#define SP_LASER    2
#define SP_ENEMY    3

#define MAX_LASERS  8
#define MAX_ENEMIES 8

#define BASE_TILE_LASER 1
#define BASE_TILE_ENEMY (BASE_TILE_LASER + MAX_LASERS)

UWORD seed;

int i;
int n;
UBYTE key;

BOOLEAN pshot;
int px, py;

BOOLEAN ls[MAX_LASERS];
int lx[MAX_LASERS];
int ly[MAX_LASERS];

BOOLEAN es[MAX_ENEMIES];
int ex[MAX_ENEMIES];
int ey[MAX_ENEMIES];


void init_screen() {
    SPRITES_8x8;

    set_sprite_data(0, 4, sprites);

    // clear OAM
    for (i = 0; i < 40; i++) {
        set_sprite_tile(i, SP_NULL);
    }

    SHOW_SPRITES;
}

void init_lasers() {
    for (i = 0; i < MAX_LASERS; i++) {
        set_sprite_tile(BASE_TILE_LASER + i, SP_LASER);

        ls[i] = FALSE;
        lx[i] = 0;
        ly[i] = 0;
    }
}

void init_enemies() {
    for (i = 0; i < MAX_ENEMIES; i++) {
        set_sprite_tile(BASE_TILE_ENEMY + i, SP_ENEMY);

        es[i] = FALSE;
        ex[i] = 0;
        ey[i] = 0;
    }
}

void init_player() {
    set_sprite_tile(0, SP_SHIP);

    px = 84;
    py = 140;
}

void handle_input() {
    key = joypad();

    if (key & J_LEFT) {
        if (px > 8) px--;
    } else if (key & J_RIGHT) {
        if (px < 160) px++;
    }

    if (key & J_UP) {
        if (py > 16) py--;
    } else if (key & J_DOWN) {
        if (py < 151) py++;
    }

    if (key & J_A && !pshot) {
        for (i = 0; i < MAX_LASERS; i++) {
            if (!ls[i]) {
                ls[i] = TRUE; lx[i] = px; ly[i] = py;
                break;
            }
        }
        pshot = TRUE;
    } else if (!(key & J_A)) {
        pshot = FALSE;
    }
}

void move_lasers() {
    for (i = 0; i < MAX_LASERS; i++) {
        if (ls[i]) {
            ly[i] -= 2;
            if (ly[i] <= 0) {
                ls[i] = FALSE;
            }
        }
    }
}

void spawn_enemies() {
    n = arand();

    if (!(n % 50)) {
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (!es[i]) {
                es[i] = TRUE;
                ex[i] = ((UINT8) arand() % 160) + 8;
                ey[i] = 0;
                break;
            }
        }
    }
}

void move_enemies() {
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (es[i]) {
            ey[i]++;
            if (ey[i] > 160) {
                es[i] = FALSE;
            }
        }
    }
}

void update_sprites() {
    move_sprite(0, px, py);

    for (i = 0; i < MAX_LASERS; i++) {
        if (ls[i]) {
            move_sprite(BASE_TILE_LASER + i, lx[i], ly[i]);
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (es[i]) {
            move_sprite(BASE_TILE_ENEMY + i, ex[i], ey[i]);
        }
    }
}


void main() {
    disable_interrupts();
    DISPLAY_OFF;

    seed = DIV_REG;
    waitpadup();
    seed |= ((UINT16)DIV_REG << 8);
    initarand(seed);

    init_screen();
    init_player();
    init_lasers();
    init_enemies();

    DISPLAY_ON;
    enable_interrupts();

    while (1) {
        wait_vbl_done();

        handle_input();

        move_lasers();

        spawn_enemies();
        move_enemies();

        update_sprites();
    }
}

/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
