/*
 * A379120  a(1) = 1; and for n > 1, a(n) is the smallest divisor d > 1 of n
 *          such that A048720(A065621(n/d),d) is equal to n.
 * https://oeis.org/A379120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = smallest divisor d > 1 of n with
 *            A048720(A065621(n/d), d) = n (carryless GF(2) product); found by
 *            search over divisors
 * Range:     u8 n <= 256, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 9926 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a379120_gf2_mul(uint64_t i, uint64_t j)
{
    uint64_t r = 0;
    while (j) {
        if (j & 1) r ^= i;
        i <<= 1;
        j >>= 1;
    }
    return r;
}

static uint64_t a379120_rev(uint64_t m)
{
    uint64_t low = m & (~m + 1);
    return m ^ ((m - low) << 1);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)1;
    for (uint64_t d = 2; d < N; d++) {
        if (N % d) continue;
        if (a379120_gf2_mul(a379120_rev(N / d), d) == N)
            return (seq_t)d;
    }
    return (seq_t)N;
}
