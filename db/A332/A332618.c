/*
 * A332618  a(n) = Sum_{d|n} lcm(d, n/d) / gcd(d, n/d).
 * https://oeis.org/A332618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * Sum_{d|n} 1 / gcd(d, n/d)^2 The implementation uses
 *            the multiplicative formula: a(p^e) = (2*p^(e+2) - p^2 - 1)/(p^2
 *            - 1) if e is even = 2*(p^(e+2) - p)/(p^2 - 1) if e is odd
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 1830, u64 n <= 9712,
 *            u128 n <= 9712, big any n
 * Helpers:   seq_pow_si
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code uses only standard integer types and the seq_t arithmetic
   operators that are provided by the build.  It does not depend on any
   of the helper functions from the utilities, so it compiles on all
   build configurations with seq_t ranging from uint8_t to arbitrary-
   precision integers. */

#include <stdint.h>
#include "../utils/seq_pow_si.h"

/* multiplicative function a(n) */
seq_t
a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but guard anyway */
        return 0;
    if (n == 1)
        return 1;

    seq_t res = 1;
    uint64_t m = (uint64_t)n;          /* work in native unsigned integer */
    uint64_t p = 2;

    while (m > 1)
    {
        /* find next prime factor */
        while (p * p <= m && m % p != 0U)
            ++p;
        if (p * p > m)
            p = m;      /* remaining prime factor */

        /* count exponent e of prime p */
        uint64_t exp = 0;
        while (m % p == 0U)
        {
            m /= p;
            ++exp;
        }

        /* compute contribution for this prime power */
        seq_t p_seq = (seq_t)p;
        seq_t p_pow_e_plus2 = seq_pow_si(p_seq, exp + 2);
        seq_t p2 = p_seq * p_seq;
        seq_t denom = p2 - (seq_t)1;
        seq_t num;

        if ((exp & 1U) == 0U) /* e even */
            num = 2 * p_pow_e_plus2 - p2 - (seq_t)1;
        else                  /* e odd */
            num = 2 * (p_pow_e_plus2 - p_seq);

        seq_t contrib = num / denom;
        res *= contrib;
    }

    return res;
}
