/*
 * A325567  a(1) = 1; for n > 1, a(n) is the largest proper divisor d of n
 *          such that A048720(A065621(d),n/d) is equal to n.
 * https://oeis.org/A325567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = largest proper divisor d of n with
 *            A048720(A065621(d), n/d) = n (search over divisors, smallest
 *            cofactor q = n/d first)
 * Range:     u8 n <= 511, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 121 terms through n = 9501 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A065621_rev(idx_t d)
{
    idx_t low = d & (~d + 1);
    return d ^ ((d - low) << 1);
}

/* carryless (GF(2)[X]) product */
static idx_t gf2_product(idx_t i, idx_t j)
{
    idx_t r = 0;
    while (j) {
        if (j & 1) r ^= i;
        i <<= 1;
        j >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t q;
    if (n <= 1) return (seq_t)1;
    for (q = 2; q <= n; q++) {
        if (n % q) continue;
        idx_t d = n / q;
        if (gf2_product(A065621_rev(d), q) == n)
            return (seq_t)d;
    }
    return (seq_t)1;
}
