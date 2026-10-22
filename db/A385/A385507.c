/*
 * A385507  a(n) = v(1 + F(2*n - 1)), where F(x) = (3*x + 1)/2^v(3*x + 1), x
 *          is any odd natural number, and v(y) is the 2-adic valuation of y.
 * https://oeis.org/A385507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v(1 + F(2*n - 1)), with F(x) = (3*x + 1)/2^v(3*x + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* x = 2*n - 1   (odd) */
    idx_t x = (idx_t)2 * n - (idx_t)1;

    /* t = 3*x + 1   (even) */
    unsigned long long t = (unsigned long long)3 * (unsigned long long)x + (unsigned long long)1;

    /* s = v_2(t) : count trailing zeros of t */
    int s = 0;
    unsigned long long tmp = t;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++s;
    }

    /* m = t >> s  (odd part of t) */
    unsigned long long m = t >> s;

    /* y = 1 + m   (even) */
    unsigned long long y = m + 1ULL;

    /* v = v_2(y) */
    int v = 0;
    tmp = y;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++v;
    }

    return (seq_t)v;
}
