/*
 * A365805  a(n) = largest exponent m for which a representation of the form
 *          A163511(n) = k^m exists (for some k). a(0) = 0 by convention.
 * https://oeis.org/A365805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052409(A163511(n)) = gcd of the exponents in the prime
 *            factorization of A163511(n); a(0) = 0. The exponent of p(i)
 *            (i-th set bit of n, counting from the low end) is the number of
 *            0-bits just below that bit, with 1 added for the last (r-th)
 *            prime p(r)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9696 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a_gcd_helper(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t g = 0;
    uint64_t prev = 0;      /* number of bit positions already consumed */
    uint64_t pos = 0;
    uint64_t tmp = (uint64_t)n;
    uint64_t last_exp = 0;

    while (tmp) {
        if (tmp & 1ULL) {
            uint64_t e = pos - prev;   /* zeros between set bits */
            prev = pos + 1;
            last_exp = e;
            if (tmp >> 1) {
                g = a_gcd_helper(g, e);
            }
        }
        ++pos;
        tmp >>= 1;
    }
    /* last prime gets one extra factor */
    g = a_gcd_helper(g, last_exp + 1);
    return (seq_t)g;
}
