/*
 * A280688  a(n) = A000045(A054576(n))= A280686(A105800(n)).
 * https://oeis.org/A280688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A054576(n)), where A054576(n) is the largest
 *            proper factor of the largest proper factor of n (1 for n = 1 or
 *            prime)
 * Range:     u8 n <= 55, u16 n <= 99, u32 n <= 191, u64 n <= 375,
 *            u128 n <= 747, big any n
 * Validated: standard, pass, 118 terms through n = 845 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A280688_lpf(idx_t n)
{
    idx_t p;
    if (n <= 1) return 1;
    for (p = 2; p <= n / p; ++p) {
        if (n % p == 0) return n / p;
    }
    return 1;
}

seq_t a(idx_t n)
{
    idx_t m = A280688_lpf(A280688_lpf(n));
    seq_t x = 0, y = 1;
    idx_t i;
    for (i = 0; i < m; i++) {
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return x;
}
