/*
 * A268462  Expansion of (2 x^4*(5 - 12*x + 8*x^2))/(1 - 2*x)^4.
 * https://oeis.org/A268462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n <= 3; a(n) = 2^(n-4) * (n-3)*(n+1)*(n+2) / 3 for
 *            n > 2 (OEIS A268462)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    /* compute (n-3)*(n+1)*(n+2) / 3  */
    seq_t prod = (seq_t)(n - 3);
    prod *= (seq_t)(n + 1);
    prod *= (seq_t)(n + 2);
    prod /= 3;             /* exact division; product always divisible by 3 */

    /* multiply by 2^(n-4) */
    return pow2(n - 4) * prod;
}
