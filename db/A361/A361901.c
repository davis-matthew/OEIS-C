/*
 * A361901  The number of linear extensions of n fork-join DAGs of width 3.
 * https://oeis.org/A361901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n)! / 20^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Compute the number of linear extensions of n fork‑join DAGs of width 3.
 *
 * The algorithm cancels factors of 2 and 5 from the factorial product
 * as soon as they match the denominator 20^n = 2^(2n) * 5^n.
 * This keeps intermediate values bounded so that the routine works
 * even when seq_t is a 64‑bit unsigned integer.
 */
seq_t a(idx_t n)
{
    /* special case n == 0 */
    if (n == 0) return (seq_t)1;

    /* number of 2‑factors and 5‑factors that must be cancelled */
    idx_t rem2 = 2 * n;  /* exponent of 2 in 20^n */
    idx_t rem5 = n;      /* exponent of 5 in 20^n */

    seq_t res = (seq_t)1;

    /* multiply numbers 1 .. 5n, removing cancelable 2s and 5s */
    for (idx_t i = 1; i <= 5 * n; ++i) {
        seq_t x = (seq_t)i;

        /* cancel 2‑factors while we still have credit */
        while ((x % (seq_t)2 == 0) && rem2 > 0) {
            x /= (seq_t)2;
            rem2--;
        }

        /* cancel 5‑factors while we still have credit */
        while ((x % (seq_t)5 == 0) && rem5 > 0) {
            x /= (seq_t)5;
            rem5--;
        }

        /* multiply the remaining part */
        res *= x;
    }

    /* At this point rem2 and rem5 should be zero because
       factorial always contains at least as many 2s and 5s as the
       denominator.  No further multiplication is needed. */

    return res;
}
