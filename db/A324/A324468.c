/*
 * A324468  a(n) = r(n) + r(n+1) + r(n+2), where r(n) is the ruler sequence
 *          A007814.
 * https://oeis.org/A324468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = r(n) + r(n+1) + r(n+2) where r(k) is the exponent of the
 *            * highest power of 2 dividing k (the ruler sequence A007814)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic on the supplied
 * types seq_t and idx_t.  No external libraries or non‑portable intrinsics
 * are required. */

#include <stdint.h>

/* Compute v2(n): number of trailing zeros in the binary representation of n.
 * n is guaranteed to be >= 1 in all calls to a(). */
static seq_t trailing_zeros(idx_t n)
{
    seq_t cnt = 0;
    /* n is unsigned, so shifting right preserves zeros. */
    while ((n & 1ULL) == 0) {
        n >>= 1;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) = r(1)+r(2)+r(3) = 0+1+0 = 1. */
    return trailing_zeros(n) + trailing_zeros(n + 1) + trailing_zeros(n + 2);
}
