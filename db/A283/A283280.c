/*
 * A283280  Number of nX6 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of n×6 binary arrays where each 1 has at most two
 *            neighbors among the horizontal (left/right), vertical (up/down)
 *            and antidiagonal (up‑right and down‑left) cells. The
 *            implementation uses DP over pairs of rows, building a
 *            compatibility table for each pair (prev, cur) that lists all
 *            rows next...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* compatibility entry for a pair of rows */
typedef struct {
    uint8_t count;          /* number of compatible next rows */
    uint8_t masks[64];      /* list of compatible next row masks */
} Compat;

/* static tables (built once) */
static Compat compat[64][64];
static bool  validEnd[64][64];
static unsigned init_done = 0;

/* check whether rows a, b, c form a valid triple for the middle row b
   (using horizontal, vertical and antidiagonal neighbors). */
static bool is_valid(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < 6; ++j) {
        if ((b >> j) & 1U) {                 /* a 1 in row b, column j */
            int total = 0;
            /* horizontal neighbours in row b */
            if (j > 0   && ((b >> (j - 1)) & 1U)) total++;
            if (j < 5   && ((b >> (j + 1)) & 1U)) total++;
            /* vertical neighbours */
            if ((a >> j) & 1U) total++;       /* above */
            if ((c >> j) & 1U) total++;       /* below */
            /* antidiagonal neighbours (up-right, down-left) */
            if (j < 5 && ((a >> (j + 1)) & 1U)) total++;   /* up‑right  */
            if (j > 0 && ((c >> (j - 1)) & 1U)) total++;   /* down‑left */
            if (total > 2) return false;
        }
    }
    return true;
}

/* initialize compatibility tables (once) */
static void init_tables(void)
{
    if (init_done) return;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            unsigned char cnt = 0;
            bool has_zero = false;
            for (int c = 0; c < 64; ++c) {
                if (is_valid((uint8_t)a, (uint8_t)b, (uint8_t)c)) {
                    compat[a][b].masks[cnt++] = (uint8_t)c;
                    if (c == 0) has_zero = true;
                }
            }
            compat[a][b].count = cnt;
            validEnd[a][b]      = has_zero;
        }
    }
    init_done = 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* state arrays: use two buffers and swap pointers */
    static seq_t dp_arr[4096];
    static seq_t next_arr[4096];
    seq_t *cur  = dp_arr;
    seq_t *nxt  = next_arr;

    /* initialise dp for the first row: previous row is zero */
    for (int i = 0; i < 4096; ++i) cur[i] = 0;
    seq_t one = 1;
    for (int mask = 0; mask < 64; ++mask) cur[mask] = one;   /* state (0,mask) */

    /* iterate over remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next buffer */
        for (int i = 0; i < 4096; ++i) nxt[i] = 0;

        for (int idx = 0; idx < 4096; ++idx) {
            seq_t val = cur[idx];
            if (val == 0) continue;
            int a_row = idx >> 6;
            int b_row = idx & 63;
            Compat *cptr = &compat[a_row][b_row];
            for (uint8_t t = 0; t < cptr->count; ++t) {
                int c_row = cptr->masks[t];
                int next_idx = (b_row << 6) | c_row;
                nxt[next_idx] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final validation of the last row with a trailing zero row */
    seq_t result = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            if (validEnd[a][b]) {
                result += cur[(a << 6) | b];
            }
        }
    }
    return result;
}
