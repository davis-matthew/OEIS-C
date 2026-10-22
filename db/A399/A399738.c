/*
 * A399738  The sum of the unitary divisors d of n such that n/d is odd.
 * https://oeis.org/A399738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p==2 ? p^e : (p^e + 1))
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(2^e)=2^e, a(p^e)=p^e+1 for odd prime p. */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    uint64_t v = (uint64_t)n;          /* work in 64‑bit integer for factorisation */
    seq_t res = 1;                     /* multiplicative accumulator */

    /* Handle factor 2 – it must be present in every allowed divisor. */
    if ((v & 1ULL) == 0) {
        unsigned e2 = 0;
        while ((v & 1ULL) == 0) {
            v >>= 1;
            ++e2;
        }
        res *= pow_uint(2ULL, e2);     /* term: 2^e2 */
    }

    /* Factor the remaining odd part by trial division. */
    uint64_t d = 3;
    while (d <= v / d) {              /* use division to avoid overflow */
        if (v % d == 0) {
            unsigned exp = 0;
            while (v % d == 0) {
                v /= d;
                ++exp;
            }
            seq_t pe = pow_uint(d, exp); /* p^e */
            seq_t term = pe + (seq_t)1;   /* p^e + 1 for odd prime */
            res *= term;
        }
        d += 2;                       /* skip even numbers */
    }

    /* If v > 1 then it is a prime factor > sqrt(original n). */
    if (v > 1) {
        seq_t pe = (seq_t)v;           /* v^1 */
        seq_t term = pe + (seq_t)1;    /* p + 1 */
        res *= term;
    }

    return res;
}
