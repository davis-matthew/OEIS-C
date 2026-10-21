/*
 * A269757  Number of black cells after n moves of Langton's ant on an
 *          infinite hexagonal grid, starting with only white cells.
 * https://oeis.org/A269757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by simulating Langton's ant on an infinite
 *            hexagonal grid starting from all white cells, counting the
 *            number of black cells after n moves. The ant rules are: on a
 *            white cell turn 60° right, flip to black and move forward; on a
 *            black cell turn 60° left, flip to white and move forw...
 * Range:     u8 n <= 4635, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hash_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/hash_coords.h"

/* Cell used in the hash table */
typedef struct {
    int32_t q, r;          /* axial coordinates */
    unsigned char color;   /* 0 = white, 1 = black */
    unsigned char used;    /* occupancy flag */
} Cell;

/* axial direction vectors for hex grid */
static const int32_t dq[6] = {  1,  0, -1, -1,  0,  1 };
static const int32_t dr[6] = {  0,  1,  1,  0, -1, -1 };

/* external function required by the build */
seq_t a(idx_t n)
{
    /* table size: power of two > 2*(n+1) to keep load factor <= 0.5 */
    size_t required = (size_t)(n + 1);
    size_t table_sz = 16;
    while (table_sz < required * 2)
        table_sz <<= 1;

    Cell *table = (Cell *)calloc(table_sz, sizeof(Cell));
    if (!table)
        abort();

    size_t mask = table_sz - 1;
    int32_t q = 0, r = 0;
    uint8_t dir = 0;          /* 0..5, 0 = (1,0) */
    seq_t black_count = 0;

    for (idx_t step = 0; step < n; ++step) {
        /* find or probe for the current cell */
        size_t h = hash_coords(q, r) & mask;
        while (table[h].used) {
            if (table[h].q == q && table[h].r == r)
                break;
            h = (h + 1) & mask;
        }

        bool is_black = false;
        if (table[h].used)
            is_black = table[h].color;

        if (is_black) {
            /* turn left, flip to white */
            dir = (dir + 5) % 6;
            table[h].color = 0;
            if (black_count)
                --black_count;
        } else {
            /* turn right, flip to black */
            dir = (dir + 1) % 6;
            if (!table[h].used) {
                table[h].used = 1;
                table[h].q = q;
                table[h].r = r;
            }
            table[h].color = 1;
            ++black_count;
        }

        /* move forward */
        q += dq[dir];
        r += dr[dir];
    }

    free(table);
    return black_count;
}
