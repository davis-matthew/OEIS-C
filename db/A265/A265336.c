/*
 * A265336  Number of nonleading 0-bits in bijective base-3 reversal of n:
 *          a(n) = A080791(A263273(n)).
 * https://oeis.org/A265336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A080791(A263273(n)); A263273 = reverse the ternary digits
 *            of n after stripping trailing zeros, then multiply back by 3^k;
 *            A080791 = number of nonleading 0-bits
 * Range:     u8 n <= 281, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 145 terms through n = 9502 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t temp = n;
    seq_t p3 = (seq_t)1;
    while ((temp % (idx_t)3) == 0) {
        temp /= (idx_t)3;
        p3 *= (seq_t)3;
    }
    seq_t rev = (seq_t)0;
    idx_t m = temp;
    while (m > 0) {
        rev = rev * (seq_t)3 + (seq_t)(m % (idx_t)3);
        m /= (idx_t)3;
    }
    seq_t x = rev * p3;

    seq_t zeros = (seq_t)0;
    while (x > (seq_t)0) {
        if ((x & (seq_t)1) == (seq_t)0)
            ++zeros;
        x >>= 1;
    }
    return zeros;
}
