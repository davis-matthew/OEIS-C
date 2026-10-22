/*
 * A382490  The number of infinitary 3-smooth divisors of n.
 * https://oeis.org/A382490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^{popcount(v2(n)) + popcount(v3(n))}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Number of infinitary 3‑smooth divisors of n.
 * For n ≥ 1: count exponent of 2 (v2) and exponent of 3 (v3).
 * Result is 1 shifted left by the sum of popcounts of v2 and v3.
 */
#include <stdint.h>

static unsigned popcnt_uint64(uint64_t x)
{
    unsigned c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, but harmless */

    /* exponent of 2 in n */
    uint64_t v2 = 0;
    uint64_t t = n;
    while ((t & 1ULL) == 0ULL) {
        ++v2;
        t >>= 1;
    }

    /* exponent of 3 in n */
    uint64_t v3 = 0;
    t = n;
    while (t % 3ULL == 0ULL) {
        ++v3;
        t /= 3ULL;
    }

    unsigned bits2 = popcnt_uint64(v2);
    unsigned bits3 = popcnt_uint64(v3);
    unsigned shift = bits2 + bits3;

    return ((seq_t)1) << shift;
}
