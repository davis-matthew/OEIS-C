/*
 * A262820  Number of (1+3)X(n+3) 0..1 arrays with each row divisible by 9 and
 *          column not divisible by 9, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq a(n): 4×(n+3) binary arrays, each row ≡0 (mod 9),
   every column≠0,9 (mod 9), read as binary numbers with top/left
   as most‑significant bits.  Computed by dynamic programming over
   column states. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define STATE_REMS 9
#define STATE_ROWS 4
#define STATE_COUNT 6561   /* 9^4 */

#define ALLOWED_COUNT 14   /* number of allowed 4‑bit patterns */
static int allowed_bits[ALLOWED_COUNT];
static int dest_table[STATE_COUNT][ALLOWED_COUNT];
static bool dest_computed = false;

static void compute_dest(void) {
    if (dest_computed) return;
    /* Build list of allowed patterns (exclude 0000 and 1001). */
    int idx = 0;
    for (int p = 0; p < 16; ++p) {
        if (p == 0 || p == 9) continue; /* forbidden columns */
        allowed_bits[idx++] = p;
    }
    for (int s = 0; s < STATE_COUNT; ++s) {
        int rem[STATE_ROWS];
        int t = s;
        for (int r = STATE_ROWS - 1; r >= 0; --r) {
            rem[r] = t % STATE_REMS;
            t /= STATE_REMS;
        }
        for (int k = 0; k < ALLOWED_COUNT; ++k) {
            int pattern = allowed_bits[k];
            int nrem[STATE_ROWS];
            for (int r = 0; r < STATE_ROWS; ++r) {
                int bit = (pattern >> (3 - r)) & 1; /* top row is bit 3 */
                nrem[r] = (2 * rem[r] + bit) % STATE_REMS;
            }
            int newstate = ((((nrem[0] * STATE_REMS + nrem[1]) * STATE_REMS
                              + nrem[2]) * STATE_REMS + nrem[3]));
            dest_table[s][k] = newstate;
        }
    }
    dest_computed = true;
}

static seq_t *dp_buf = NULL;
static seq_t *newdp_buf = NULL;
static size_t dp_buf_size = 0;

seq_t a(idx_t n) {
    /* Column count */
    idx_t cols = n + 3;

    compute_dest();

    /* Allocate DP buffers once */
    if (dp_buf_size < STATE_COUNT) {
        if (dp_buf) free(dp_buf);
        if (newdp_buf) free(newdp_buf);
        dp_buf = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);
        newdp_buf = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);
        dp_buf_size = STATE_COUNT;
    }

    /* Initialize DP */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_buf[i] = 0;
    dp_buf[0] = 1; /* start from zero remainder state */

    /* Iterate over columns */
    for (idx_t step = 0; step < cols; ++step) {
        for (size_t i = 0; i < STATE_COUNT; ++i) newdp_buf[i] = 0;
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_buf[s];
            if (!val) continue;
            /* Unroll the 14 allowed patterns for speed */
            int d0  = dest_table[s][0],  d1  = dest_table[s][1],
                d2  = dest_table[s][2],  d3  = dest_table[s][3],
                d4  = dest_table[s][4],  d5  = dest_table[s][5],
                d6  = dest_table[s][6],  d7  = dest_table[s][7],
                d8  = dest_table[s][8],  d9  = dest_table[s][9],
                d10 = dest_table[s][10], d11 = dest_table[s][11],
                d12 = dest_table[s][12], d13 = dest_table[s][13];
            newdp_buf[d0]  += val; newdp_buf[d1]  += val;
            newdp_buf[d2]  += val; newdp_buf[d3]  += val;
            newdp_buf[d4]  += val; newdp_buf[d5]  += val;
            newdp_buf[d6]  += val; newdp_buf[d7]  += val;
            newdp_buf[d8]  += val; newdp_buf[d9]  += val;
            newdp_buf[d10] += val; newdp_buf[d11] += val;
            newdp_buf[d12] += val; newdp_buf[d13] += val;
        }
        /* Swap buffers */
        seq_t *tmp = dp_buf; dp_buf = newdp_buf; newdp_buf = tmp;
    }

    return dp_buf[0];
}
