/*
 * A374647  a(n) = n! / floor(2n/3)!.
 * https://oeis.org/A374647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / floor(2n/3)!
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = floor(2n/3) without overflow using integer arithmetic */
    idx_t q = n / 3;
    idx_t r = n % 3;
    idx_t m = 2 * q + (r == 2 ? 1 : 0);

    /* If m >= n, the product is empty; return 1 (covers n = 0) */
    if (m >= n)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = m + 1; k <= n; ++k)
        result *= (seq_t)k;

    return result;
}
