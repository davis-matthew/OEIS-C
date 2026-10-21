/*
 * A269652  Number of length-n 0..4 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A269652
    Count of length‑n arrays over {0,1,2,3,4} with no repeated ascending pair
    (x,x+1).  This is computed by dynamic programming over the
    5 symbols and a 4‑bit mask tracking which ascending pairs have
    already appeared.

    The recurrence is implicit: for n≥2
        a(n) = sum over all states s,mask of   count(s,mask)
        where count(s,mask) is produced by transitions that
        either leave the mask unchanged (non‑ascending pair) or
        set a yet‑unused ascending‑pair bit.
    Initial state: for each symbol s∈{0,1,2,3,4} we have one sequence
    of length 1 with mask 0. */

#include <stdint.h>
#include <stdbool.h>

static const int pair_bit[5][5] = {
    { -1,  0, -1, -1, -1 },   /* from 0   */
    { -1, -1,  1, -1, -1 },   /* from 1   */
    { -1, -1, -1,  2, -1 },   /* from 2   */
    { -1, -1, -1, -1,  3 },   /* from 3   */
    { -1, -1, -1, -1, -1 }    /* from 4   */
};

seq_t a(idx_t n)
{
    /* Number of states: 5 symbols × 2^4 masks = 80 */
    seq_t cur[80]  = {0};
    seq_t next[80] = {0};

    /* Initial sequences of length 1: one for each symbol, mask 0 */
    for (int s = 0; s < 5; ++s)
        cur[s * 16 + 0] = 1;

    /* Transition for each additional position */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next array */
        for (int i = 0; i < 80; ++i)
            next[i] = 0;

        /* iterate over previous states */
        for (int s = 0; s < 5; ++s) {
            for (int m = 0; m < 16; ++m) {
                seq_t count = cur[s * 16 + m];
                if (count == 0) continue;
                for (int t = 0; t < 5; ++t) {
                    int bit = pair_bit[s][t];
                    if (bit >= 0) {            /* ascending pair */
                        if (m & (1 << bit)) continue;  /* already used */
                        int new_mask = m | (1 << bit);
                        next[t * 16 + new_mask] += count;
                    } else {                 /* any other pair */
                        next[t * 16 + m] += count;
                    }
                }
            }
        }
        /* swap cur and next */
        for (int i = 0; i < 80; ++i)
            cur[i] = next[i];
    }

    /* Sum all sequences of length n */
    seq_t total = 0;
    for (int i = 0; i < 80; ++i)
        total += cur[i];

    return total;
}
