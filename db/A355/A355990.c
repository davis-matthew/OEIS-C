/*
 * A355990  a(n) = n! / (6 * floor(n/3)!).
 * https://oeis.org/A355990
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / (6 * floor(n/3)!)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* behaviour below offset does not matter */

    idx_t m   = n / 3;            /* floor(n/3) */
    seq_t prod = 1;               /* product of remaining factors */
    seq_t rem  = 6;               /* divide by 6 at the end */

    for (idx_t k = m + 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t g  = gcd(kk, rem);
        if (g > 1) {
            kk /= g;             /* cancel part of the divisor */
            rem /= g;
        }
        prod *= kk;              /* multiply the reduced factor */
    }

    /* rem should be 1 here; if not, the product is divisible by 6 anyway */
    return prod;
}
