/*
 * A261990  Color of the cell Langton's Ant touches on its n-th step before
 *          the color is changed by the ant; 0=white, 1=black.
 * https://oeis.org/A261990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = color of the cell Langton’s Ant touches on its n‑th step
 *            before the color is changed by the ant; 0 = white, 1 = black
 * Range:     u8 n <= 9083, u16 n <= 9083, u32 n <= 9083, u64 n <= 9083,
 *            u128 n <= 9083, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int64_t x;
    int64_t y;
    uint8_t used;   /* 0 = empty, 1 = present */
    uint8_t color;  /* current color, 0 or 1 */
} cell_t;

static inline uint64_t hash64(int64_t x, int64_t y)
{
    const uint64_t mul1 = 11400714819323198485ull; /* splitmix64 constant */
    const uint64_t mul2 = 14029467366897019727ull;
    return (uint64_t)(x * mul1) ^ (uint64_t)(y * mul2);
}

static inline int find_index(cell_t *table, size_t mask,
                             int64_t x, int64_t y, int *found)
{
    uint64_t h = hash64(x, y) & mask;
    for (;;) {
        if (!table[h].used) {
            *found = 0;
            return (int)h;
        }
        if (table[h].x == x && table[h].y == y) {
            *found = 1;
            return (int)h;
        }
        h = (h + 1) & mask;
    }
}

static inline uint8_t lookup_and_flip(cell_t *table, size_t mask,
                                      int64_t x, int64_t y)
{
    int hit;
    int pos = find_index(table, mask, x, y, &hit);
    if (hit) {
        table[pos].color ^= 1;              /* flip color */
        return table[pos].color ^ 1;        /* old color */
    } else {
        table[pos].used  = 1;
        table[pos].x     = x;
        table[pos].y     = y;
        table[pos].color = 1;               /* white -> black */
        return 0;                           /* old color was 0 */
    }
}

seq_t a(idx_t n)
{
    /* Ensure capacity for up to n+1 distinct cells (worst case). */
    size_t cap = 1;
    for (size_t t = n + 2; t > 0; t >>= 1) cap <<= 1;
    if (cap < 2) cap = 2;                     /* never zero */
    uint64_t mask = (uint64_t)cap - 1;

    cell_t *table = (cell_t*)malloc(cap * sizeof(cell_t));
    if (!table) return (seq_t)0;              /* out‑of‑memory fallback */

    /* Mark all entries unused. */
    for (size_t i = 0; i < cap; ++i) table[i].used = 0;

    /* Directions: 0=N, 1=E, 2=S, 3=W */
    static const int dx[4] = {0, 1, 0, -1};
    static const int dy[4] = {1, 0, -1, 0};

    int64_t x = 0, y = 0;
    int dir = 1;                 /* start facing East */
    seq_t result = 0;

    for (idx_t step = 0; step <= n; ++step) {
        uint8_t clr = lookup_and_flip(table, mask, x, y);
        if (step == n) {
            result = (seq_t)clr;
            break;
        }
        /* Determine turn based on current color. */
        dir = (clr == 0) ? (dir + 1) & 3  /* white: right */:
                         (dir + 3) & 3;    /* black: left */
        /* Move forward. */
        x += dx[dir];
        y += dy[dir];
    }

    free(table);
    return result;
}
