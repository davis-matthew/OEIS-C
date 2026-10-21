/*
 * A244040  Sum of digits of n in fractional base 3/2.
 * https://oeis.org/A244040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(2*floor(n/3)) + n%3 Computed iteratively: a(n) = Σ (n_i
 *            mod 3) where n_{i+1} = 2 * floor(n_i/3), start n_0 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n) {
        sum += (seq_t)(n % 3u);
        n = (n / 3u) * 2u;
    }
    return sum;
}
