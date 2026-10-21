/*
 * A241933  Number of length n+4 0..5 arrays with no consecutive five elements
 *          summing to more than 2*5.
 * https://oeis.org/A241933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over length n+4 arrays of digits 0..5,
   keeping the last 4 digits as state.  For each new digit
   the sum of the last 4 plus the new digit must not exceed 10. */

#include <stdint.h>
#include <string.h>

static uint8_t  sum4[1296];
static uint16_t mod216[1296];
static int      init_done = 0;

static seq_t   arr0[1296];
static seq_t   arr1[1296];
static seq_t  *curr  = arr0;
static seq_t  *next  = arr1;

void compute_state_tables(void) {
    for (idx_t a0 = 0; a0 < 6; a0++)
        for (idx_t a1 = 0; a1 < 6; a1++)
            for (idx_t a2 = 0; a2 < 6; a2++)
                for (idx_t a3 = 0; a3 < 6; a3++) {
                    idx_t s = a0 * 216 + a1 * 36 + a2 * 6 + a3;
                    sum4[s]  = a0 + a1 + a2 + a3;
                    mod216[s] = s % 216;
                }
    init_done = 1;
}

seq_t a(idx_t n) {
    if (!init_done) compute_state_tables();

    /* L = n + 4 is the length of the array */
    /* We perform n steps starting from all 4‑digit prefixes */
    for (idx_t i = 0; i < 1296; i++) curr[i] = 1;

    for (idx_t step = 0; step < n; step++) {
        /* clear next */
        for (idx_t i = 0; i < 1296; i++) next[i] = 0;

        for (idx_t s = 0; s < 1296; s++) {
            seq_t cnt = curr[s];
            if (!cnt) continue;
            uint8_t ssum = sum4[s];

            /* try all possible next digits */
            for (uint8_t x = 0; x < 6; x++) {
                if (ssum + x <= 10) {
                    idx_t ns = mod216[s] * 6 + x; /* shift state, append x */
                    next[ns] += cnt;
                }
            }
        }
        /* swap curr and next */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* sum all counts in curr to get the total */
    seq_t total = 0;
    for (idx_t i = 0; i < 1296; i++) total += curr[i];
    return total;
}
