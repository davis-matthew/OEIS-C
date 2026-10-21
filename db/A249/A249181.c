/*
 * A249181  a(n) = A057137(n)^2 where A057137 = 0,1,12,123,...,123...90,...
 * https://oeis.org/A249181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A057137(n))^2, where A057137(n) is the integer formed by
 *            the first n digits of the repeating sequence 1234567890
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t d = (seq_t)(i % 10);  /* digit 1-9 or 0 */
        val = val * 10 + d;
    }
    return val * val;
}
