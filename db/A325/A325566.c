/*
 * A325566  a(n) is the largest divisor d of n such that
 *          A048720(A065621(d),n/d) = n.
 * https://oeis.org/A325566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor d of n with A048720(A065621(d), n/d) = n;
 *            found by scanning divisors downward
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 121 terms through n = 9939 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A065621_rev(idx_t d)
{
    seq_t x = (seq_t)d;
    seq_t low = (seq_t)(d & (~d + 1));
    return x ^ ((x - low) << 1);
}

static seq_t A048720_mul(seq_t i, idx_t j)
{
    seq_t result = 0;
    seq_t shifted = i;
    while (j) {
        if (j & 1)
            result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t d;
    for (d = n; d >= 1; d--) {
        if (n % d != 0)
            continue;
        if (A048720_mul(A065621_rev(d), n / d) == (seq_t)n)
            return (seq_t)d;
    }
    return 1;
}
