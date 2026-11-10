/*
 * A325570  Numbers n that have no divisor d > 1 such that
 *          A048720(A065621(d),n/d) = n.
 * https://oeis.org/A325570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with no divisor d>1 such that
 *            A048720(A065621(d), m/d) = m (search over m, testing every
 *            divisor d>1)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9225 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128
A325570_gf2_mul(uint64_t x, uint64_t y)
{
    unsigned __int128 r = 0, s = x;
    while (y) {
        if (y & 1) r ^= s;
        s <<= 1;
        y >>= 1;
    }
    return r;
}

static uint64_t
A325570_rev(uint64_t d)
{
    uint64_t low = d & (~d + 1);
    return d ^ ((d - low) << 1);
}

static int
A325570_test(uint64_t m, uint64_t d)
{
    if (d <= 1) return 0;
    return A325570_gf2_mul(A325570_rev(d), m / d) == (unsigned __int128)m;
}

static int
A325570_good(uint64_t m)
{
    uint64_t i;
    for (i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            if (A325570_test(m, i)) return 0;
            if (A325570_test(m, m / i)) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t m = 0;
    uint64_t count = 0;
    while (count < (uint64_t)n) {
        m++;
        if (A325570_good(m)) count++;
    }
    return (seq_t)m;
}
