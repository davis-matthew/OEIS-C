/*
 * A355653  For any number n with runs in binary expansion (r_w, ..., r_0),
 *          let p(n) be the polynomial of a single indeterminate x where the
 *          coefficient of x^e is r_e for e = 0..w and otherwise 0, and let q
 *          be the inverse of p; a(n) = q(p(n)').
 * https://oeis.org/A355653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = q(p(n)') where p is the polynomial with coefficients
 *            equal to the lengths of the runs of 1's and 0's in the binary
 *            expansion of n. To compute a(n) we use the equivalent
 *            construction: - Parse the runs of n starting at the
 *            least‑significant bit. - For e = 1 … w (w = number of runs – 1)
 *            compu...
 * Range:     u8 n <= 20, u16 n <= 73, u32 n <= 297, u64 n <= 2729,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    /* Special cases */
    if (n <= 1) return (seq_t)0;

    /* Collect runs of 1's and 0's, starting from the LSB.
       run_len[i]  – length of the run with index i
       run_bit[i]  – bit value of that run (0 or 1)          */
    unsigned run_len[65];      /* at most 65 runs for a 64‑bit n */
    unsigned run_bit[65];
    int rc = 0;                /* number of runs */

    int cur_bit = n & 1;
    idx_t tmp = n;

    while (tmp) {
        unsigned cnt = 0;
        while (tmp && ((tmp & 1) == cur_bit)) {
            cnt++;
            tmp >>= 1;
        }
        run_len[rc] = cnt;
        run_bit[rc] = cur_bit;
        rc++;
        cur_bit ^= 1;
    }

    /* w is the index of the highest non‑zero run */
    int w = rc - 1;
    if (w <= 0) return (seq_t)0;   /* should not happen for n > 1 */

    seq_t result = (seq_t)0;

    /* Process runs from w down to 1 */
    for (int e = w; e >= 1; --e) {
        /* New run length = e * original run length */
        idx_t block_len = (idx_t)(e * (idx_t)run_len[e]);

        /* Multiply current result by 2^block_len (shift left) */
        seq_t shift_factor = pow2(block_len);
        result = result * shift_factor;

        /* If the run is a block of 1's, add the mask of ones */
        if (run_bit[e]) {
            seq_t mask = shift_factor - (seq_t)1;
            result += mask;
        }
    }

    return result;
}
