/*
 * A246674  Run Length Transform of A000225.
 * https://oeis.org/A246674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{r∈{lengths of runs of 1's in binary(n)}} (2^r - 1)
 * Range:     u8 n <= 510, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* product of terms (empty product = 1)   */
    while (n) {
        /* skip trailing zeros (least‑significant bits) */
        while ((n & 1) == 0) {
            n >>= 1;
            if (!n) goto finished;
        }
        /* count consecutive 1's - this gives the run length   */
        idx_t run = 0;
        while ((n & 1) == 1) {
            ++run;
            n >>= 1;
        }
        /* multiply the contribution (2^run - 1) */
        prod *= (pow2(run) - (seq_t)1);
    }
finished:
    return prod;
}
