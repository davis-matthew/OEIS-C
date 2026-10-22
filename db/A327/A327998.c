/*
 * A327998  a(n) = (n!/floor(n/2)!^2)^2.
 * https://oeis.org/A327998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! / floor(n/2)!^2)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    idx_t k = n >> 1;
    /* binom returns n! / (k! * (n-k)!) */
    seq_t v = binom(n, k);
    if (n & 1U) /* odd n: multiply by (n-k) to turn (n-k)! into k! */
    {
        v *= (seq_t)(n - k);
    }
    return v * v;
}
