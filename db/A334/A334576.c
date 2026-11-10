/*
 * A334576  a(n) is the X-coordinate of the n-th point of the space filling
 *          curve P defined in Comments section; sequence A334577 gives
 *          Y-coordinates.
 * https://oeis.org/A334576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n+1) = (A020986(n) + A020986(n+1) - 1)/2 for n >= 0,
 *            where A020986(n) = sum_{k=0..n} (-1)^popcount(k & (k>>1))
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 93 terms through n = 4026 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A334576_parity(idx_t x)
{
    int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* compute S(n-1) and S(n) */
    seq_t sum = 0;
    seq_t prev = 0;
    for (idx_t k = 0; k <= n; ++k) {
        if (k == n)
            prev = sum;
        if (A334576_parity(k & (k >> 1)) == 0)
            sum += (seq_t)1;
        else
            sum -= (seq_t)1;
    }
    return (prev + sum - (seq_t)1) / (seq_t)2;
}
