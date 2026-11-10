/*
 * A264978  Self-inverse permutation of nonnegative integers: a(n) =
 *          A263273(8*n)/8.
 * https://oeis.org/A264978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A263273(8*n)/8, where A263273(m) is the bijective base-3
 *            reversal of m: strip the factors of 3 from m, reverse the base-3
 *            digits of the rest, and multiply the factors of 3 back in
 * Range:     u8 n <= 30, u16 n <= 7381, u32 n <= 7381, u64 n <= 7381,
 *            u128 n <= 7381, big any n
 * Validated: standard, pass, 98 terms through n = 6670 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A263273_a(idx_t n)
{
    if (n == 0)
        return 0;
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
    return rev * p3;
}

seq_t a(idx_t n)
{
    return A263273_a(n * (idx_t)8) / (seq_t)8;
}
