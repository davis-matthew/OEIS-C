/*
 * A265339  a(n) = A263273(A004526(A263273(n))).
 * https://oeis.org/A265339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A263273(A004526(A263273(n))), with A004526(m) =
 *            floor(m/2) and A263273 the bijective base-3 reverse
 * Range:     u8 n <= 246, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9500 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A263273_f(seq_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t temp = n;
    seq_t p3 = (seq_t)1;
    while ((temp % 3) == 0) {
        temp /= 3;
        p3 *= 3;
    }
    seq_t rev = (seq_t)0;
    seq_t m = temp;
    while (m > 0) {
        rev = rev * 3 + (m % 3);
        m /= 3;
    }
    return rev * p3;
}

seq_t a(idx_t n)
{
    seq_t x = A263273_f((seq_t)n);
    x = x / 2;
    return A263273_f(x);
}
