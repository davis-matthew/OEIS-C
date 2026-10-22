/*
 * A397560  a(n) = ceiling(8*n/7) + [n == 6 (mod 7)].
 * https://oeis.org/A397560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(8*n/7) + [n == 6 (mod 7)]
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute ceil(8*n/7) using integer arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t result = (nn * 8 + 6) / 7;

    /* Add 1 if n ≡ 6 (mod 7). */
    if (n % 7 == 6)
        result += 1;

    return result;
}
