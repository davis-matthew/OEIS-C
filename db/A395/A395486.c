/*
 * A395486  Consider the square spiral with its cells numbered starting at 0.
 *          Two players, Black and Red, take turns. When it is Black's turn,
 *          he places a knight at the smallest unoccupied cell not attacked by
 *          an existing Red knight, and when it is Red's turn, she places a
 *          knight at the smallest unoccupied c...
 * https://oeis.org/A395486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   This function simulates the 2‑player knight‑placement game on
 *            the square spiral. a(n) is the signed ordinal of the knight
 *            placed at index n, or 0 if no knight occupies that cell
 * Range:     i8 n <= 409, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   idx_to_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* idx_t, seq_t are provided by the build system */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/idx_to_xy.h"

#define MAX_N 40000                            /* highest index that will be simulated */
#define BUF   200                              /* half‑width of the coordinate grid */
#define WIDTH (2*BUF+1)                        /* full grid side length */

/* Global simulation state ----------------------------------------------------- */
static seq_t seq_arr[MAX_N+1];                /* sequence values */
static bool   occupied[MAX_N+1];              /* cell occupancy */
static bool   attacked_red[MAX_N+1];          /* cells attacked by a red knight */
static bool   attacked_black[MAX_N+1];        /* cells attacked by a black knight */
static int32_t ix_arr[MAX_N+1], iy_arr[MAX_N+1];  /* coordinates of each index */
static idx_t coord_to_index[WIDTH*WIDTH];    /* reverse lookup: (x,y) → idx */
static bool   init_done = false;              /* has init() been run? */

/* Knight moves --------------------------------------------------------------- */
static const int8_t moves[8][2] = {
    {  1,  2}, {  2,  1}, { -1,  2}, { -2,  1},
    {  1, -2}, {  2, -1}, { -1, -2}, { -2, -1}
};

/* --------------------------------------------------------------------------- */
/* Index (x,y) → packed grid offset; return -1 if outside grid ----------------- */
static inline int idx_pack(int32_t x, int32_t y)
{
    int ix = (int)(x + BUF);
    int iy = (int)(y + BUF);
    if (ix < 0 || ix >= WIDTH || iy < 0 || iy >= WIDTH) return -1;
    return ix * WIDTH + iy;
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* One‑time initialization and simulation ------------------------------------- */
static void init(void)
{
    if (init_done) return;

    /* clear arrays */
    memset(seq_arr,    0, sizeof(seq_arr));
    memset(occupied,   0, sizeof(occupied));
    memset(attacked_red,   0, sizeof(attacked_red));
    memset(attacked_black, 0, sizeof(attacked_black));
    for (size_t i = 0; i < (size_t)WIDTH*WIDTH; ++i) coord_to_index[i] = (idx_t)-1;

    /* pre‑compute coordinates and reverse mapping */
    for (idx_t i = 0; i <= (idx_t)MAX_N; ++i) {
        int32_t x, y;
        idx_to_xy(i, &x, &y);
        ix_arr[i] = x;
        iy_arr[i] = y;
        int key = idx_pack(x, y);
        if (key >= 0) coord_to_index[key] = i;
    }

    /* Game simulation -------------------------------------------------------- */
    idx_t cur[2] = {0, 0};      /* next candidate index for each color */
    seq_t black_k = 0, red_k = 0;
    size_t turn = 0;            /* 0 = black, 1 = red */

    while (cur[0] <= (idx_t)MAX_N || cur[1] <= (idx_t)MAX_N) {
        if (cur[turn] > (idx_t)MAX_N) { turn ^= 1; continue; }

        idx_t idx = cur[turn];
        /* Find smallest admissible cell */
        while (idx <= (idx_t)MAX_N &&
               (occupied[(size_t)idx] ||
                (turn == 0 && attacked_red[(size_t)idx]) ||
                (turn == 1 && attacked_black[(size_t)idx]))) {
            ++idx;
        }

        if (idx > (idx_t)MAX_N) {
            cur[turn] = idx;
            turn ^= 1;
            continue;
        }

        /* Place the knight */
        occupied[(size_t)idx] = true;
        if (turn == 0) {            /* black */
            ++black_k;
            seq_arr[(size_t)idx] = black_k;
        } else {                    /* red */
            ++red_k;
            seq_arr[(size_t)idx] = -(red_k);
        }

        /* Mark cells attacked by this knight for the opponent */
        int32_t cx = ix_arr[(size_t)idx];
        int32_t cy = iy_arr[(size_t)idx];
        for (int m = 0; m < 8; ++m) {
            int32_t nx = cx + moves[m][0];
            int32_t ny = cy + moves[m][1];
            int key = idx_pack(nx, ny);
            if (key >= 0) {
                idx_t nid = coord_to_index[(size_t)key];
                if (nid != (idx_t)-1 && nid <= (idx_t)MAX_N) {
                    if (turn == 0) attacked_black[(size_t)nid] = true;  /* black attacks opponent */
                    else           attacked_red[(size_t)nid] = true;   /* red attacks opponent  */
                }
            }
        }

        cur[turn] = idx + 1;
        turn ^= 1;
    }

    init_done = true;
}

/* --------------------------------------------------------------------------- */
/* Public sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init();
    if (n > (idx_t)MAX_N) return 0;   /* out of pre‑simulated range */
    return seq_arr[(size_t)n];
}
