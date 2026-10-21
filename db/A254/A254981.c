/*
 * A254981  a(n) is the sum of the divisors d of n such that n/d is cubefree.
 * https://oeis.org/A254981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Sum_{d | n, n/d cubefree} d (OEIS A254981)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division and uses the
   known multiplicative formulas:
        a(p)   = 1 + p
        a(p^e) = p^{e‑2} * (1 + p + p^2)   (e > 1)           */
#include <stdint.h>
#include <stddef.h>

static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t res = (seq_t)1;
    for (uint64_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but safe */
    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor out powers of 2 */
    uint64_t e = 0;
    while ((nn & 1ULL) == 0) {
        ++e;
        nn >>= 1;
    }
    if (e) {
        seq_t p = (seq_t)2;
        seq_t part;
        if (e == 1)
            part = (seq_t)1 + p;
        else {
            seq_t p_pow = pow_seq_t(p, e - 2);
            seq_t sum = (seq_t)1 + p + p * p;
            part = p_pow * sum;
        }
        result *= part;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p != 0) continue;
        e = 0;
        while (nn % p == 0) {
            ++e;
            nn /= p;
        }
        seq_t sp = (seq_t)p;
        seq_t part;
        if (e == 1)
            part = (seq_t)1 + sp;
        else {
            seq_t p_pow = pow_seq_t(sp, e - 2);
            seq_t sum = (seq_t)1 + sp + sp * sp;
            part = p_pow * sum;
        }
        result *= part;
    }

    /* remaining prime factor > sqrt(original n) */
    if (nn > 1) {
        seq_t sp = (seq_t)nn;
        seq_t part = (seq_t)1 + sp;   /* exponent is 1 */
        result *= part;
    }

    return result;
}
