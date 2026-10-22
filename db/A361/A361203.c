/*
 * A361203  a(n) = n*A010888(n).
 * https://oeis.org/A361203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(n - 9*floor((n-1)/9)) for n > 0, a(0) = 0
 * Range:     u8 n <= 34, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* Compute (n-1) mod 9 + 1 */
    idx_t k = ((n - 1) % 9) + 1;
    return (seq_t)n * (seq_t)k;
}
