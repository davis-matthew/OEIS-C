/*
 * A246160  Inverse function to the injection A065621.
 * https://oeis.org/A246160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010060(n) * A065620(n); A065620(n) is the alternating
 *            sum of the powers of 2 in n's binary expansion, starting with +
 *            at the lowest set bit
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 101 terms through n = 6329 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* A010060(n): parity of popcount; if even, a(n) = 0. */
    if ((__builtin_popcountll((unsigned long long)n) & 1) == 0)
        return (seq_t)0;

    seq_t pos_sum = 0, neg_sum = 0;
    int sign = 1;
    idx_t x = n;
    while (x != 0) {
        idx_t mask = x & (~x + 1);
        int p = __builtin_ctzll((unsigned long long)mask);
        seq_t term = ((seq_t)1) << p;
        if (sign > 0) pos_sum += term;
        else          neg_sum += term;
        x -= mask;
        sign = -sign;
    }
    /* Odd popcount means the alternating sum is positive. */
    return pos_sum - neg_sum;
}
