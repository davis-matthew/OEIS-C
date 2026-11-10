/*
 * A325565  a(n) is the number of such divisors d of n that
 *          A048720(A065621(d),n/d) is equal to n.
 * https://oeis.org/A325565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A048720(A065621(d), n/d) == n]; divisors found
 *            by trial division up to sqrt(n), carryless product in 128 bits
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9755 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 gf_u128_t;

static gf_u128_t A325565_clmul(gf_u128_t x, uint64_t y)
{
    gf_u128_t r = 0;
    while (y) {
        if (y & 1)
            r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static int A325565_test(uint64_t d, uint64_t n)
{
    uint64_t low = d & (0 - d);
    gf_u128_t t = (gf_u128_t)d ^ (((gf_u128_t)(d - low)) << 1);
    uint64_t u = n / d;
    return A325565_clmul(t, u) == (gf_u128_t)n;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    uint64_t i;
    if (N == 0)
        return (seq_t)0;
    for (i = 1; i <= N / i; i++) {
        if (N % i == 0) {
            uint64_t j = N / i;
            count += (uint64_t)A325565_test(i, N);
            if (j != i)
                count += (uint64_t)A325565_test(j, N);
        }
    }
    return (seq_t)count;
}
