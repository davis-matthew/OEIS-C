/*
 * A371638  a(n) = 2*n + valuation(n, 3) with valuation(n, 3) = A007949(n).
 * https://oeis.org/A371638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + valuation(n, 3)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    seq_t result = (seq_t)2 * val;

    idx_t r = n;
    unsigned int cnt = 0;
    while (r % 3 == 0) {
        r /= 3;
        ++cnt;
    }

    result += (seq_t)cnt;
    return result;
}
