/*
 * A338290  Squares visited by either knight when a white knight and a black
 *          knight are moving on a spirally numbered board, always to the
 *          lowest available unvisited square; white moves first.
 * https://oeis.org/A338290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 3760, u32 n <= 3760, u64 n <= 3760,
 *            u128 n <= 3760, big any n
 * Helpers:   spiral_number_from_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338290: squares visited by either knight on a spirally numbered board.
   The function simulates the knights.  a(1)=1 (white starts), a(2)=10
   (black starts).  For a given n the function returns the nth square
   occupied by either knight. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/spiral_number_from_coord.h"

/* --------- main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    /* first two terms are fixed */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    /* visited set: dynamic boolean array */
    size_t visited_cap = 1024;
    bool *visited = (bool *)calloc(visited_cap, sizeof(bool));
    if (!visited)
        return (seq_t)0;   /* unlikely, defensive fallback */

    /* ensure capacity for the initial black square */
    if (10 >= visited_cap) {
        size_t old_cap = visited_cap;
        while (10 >= visited_cap) visited_cap <<= 1;
        bool *tmp = (bool *)realloc(visited, visited_cap * sizeof(bool));
        if (!tmp) { free(visited); return (seq_t)0; }
        visited = tmp;
        memset(visited + old_cap, 0,
               (visited_cap - old_cap) * sizeof(bool));
    }
    visited[1]  = true;
    visited[10] = true;

    /* knight positions as (x,y) coordinates */
    int32_t wx = 0, wy = 0;   /* white starts at 1 */
    int32_t bx = 2, by = -1;  /* black starts at 10 */

    idx_t result = 10;        /* last number produced */
    idx_t idx    = 2;         /* number of terms produced so far */
    bool white_turn = true;   /* next move is white */

    /* simulation loop */
    while (idx < n) {
        int32_t curx = white_turn ? wx : bx;
        int32_t cury = white_turn ? wy : by;

        idx_t best_num = (idx_t)-1;
        int32_t bestx = 0, besty = 0;

        /* examine the 8 legal knight moves */
        for (int m = 0; m < 8; ++m) {
            int32_t cx, cy;
            switch (m) {
                case 0: cx = curx + 1; cy = cury + 2; break;
                case 1: cx = curx + 1; cy = cury - 2; break;
                case 2: cx = curx - 1; cy = cury + 2; break;
                case 3: cx = curx - 1; cy = cury - 2; break;
                case 4: cx = curx + 2; cy = cury + 1; break;
                case 5: cx = curx + 2; cy = cury - 1; break;
                case 6: cx = curx - 2; cy = cury + 1; break;
                case 7: cx = curx - 2; cy = cury - 1; break;
            }

            idx_t num = spiral_number_from_coord(cx, cy);

            /* grow visited array if needed */
            if ((size_t)num >= visited_cap) {
                size_t old_cap = visited_cap;
                while ((size_t)num >= visited_cap) visited_cap <<= 1;
                bool *tmp = (bool *)realloc(visited,
                                            visited_cap * sizeof(bool));
                if (!tmp) { free(visited); return (seq_t)0; }
                visited = tmp;
                memset(visited + old_cap, 0,
                       (visited_cap - old_cap) * sizeof(bool));
            }

            if (!visited[(size_t)num] && num < best_num) {
                best_num = num;
                bestx = cx;
                besty = cy;
            }
        }

        /* update state */
        visited[(size_t)best_num] = true;
        result = best_num;
        if (white_turn) { wx = bestx; wy = besty; }
        else            { bx = bestx; by = besty; }

        ++idx;
        white_turn = !white_turn;
    }

    free(visited);
    return (seq_t)result;
}
