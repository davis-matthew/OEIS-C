/*
 * A348984  a(n) = gcd(sigma(n), A325973(n)), where A325973 is the arithmetic
 *          mean of {sum of squarefree divisors} and {sum of unitary
 *          divisors}.
 * https://oeis.org/A348984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), A325973(n)) where A325973(n) = ( sum of
 *            squarefree divisors + sum of unitary divisors ) / 2
 * Range:     u8 n <= 16, u16 n <= 2212, u32 n <= 9058, u64 n <= 9058,
 *            u128 n <= 9058, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation uses a simple trial‑division factorisation.
   It works for n up to about 10,000 in well under 2 s per call
   and relies only on standard integer arithmetic and the supplied gcd. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static void factor_simple(uint64_t n,
                          uint64_t primes[64],
                          uint64_t exps[64],
                          size_t *cnt)
{
    size_t c = 0;
    uint64_t t = n;

    if (t % 2ULL == 0ULL) {
        uint64_t e = 0;
        while (t % 2ULL == 0ULL) {
            t /= 2ULL;
            e++;
        }
        primes[c] = 2ULL;
        exps[c]   = e;
        c++;
    }
    for (uint64_t p = 3ULL; p * p <= t; p += 2ULL) {
        if (t % p == 0ULL) {
            uint64_t e = 0;
            while (t % p == 0ULL) {
                t /= p;
                e++;
            }
            primes[c] = p;
            exps[c]   = e;
            c++;
        }
    }
    if (t > 1ULL) {
        primes[c] = t;
        exps[c]   = 1;
        c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (n > 0ULL)
        factor_simple((uint64_t)n, primes, exps, &cnt);

    /* Sum of squarefree divisors: product (1 + p)       */
    seq_t s1 = 1;
    /* Sum of unitary divisors: product (1 + p^e)        */
    seq_t s2 = 1;
    /* Sum of all divisors sigma(n) = product (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_seq = (seq_t)primes[i];
        int e = (int)exps[i];

        /* term for squarefree sum */
        s1 *= (p_seq + 1);

        /* compute p^e */
        seq_t p_pow = 1;
        for (int j = 0; j < e; ++j)
            p_pow *= p_seq;

        /* term for unitary sum */
        s2 *= (p_pow + 1);

        /* term for sigma */
        seq_t numerator   = p_pow * p_seq - 1;   /* p^(e+1)-1 */
        seq_t denominator = p_seq - 1;           /* p-1 */
        seq_t term = numerator / denominator;    /* exact division */
        sigma *= term;
    }

    /* Arithmetic mean of the two sums */
    seq_t avg = (s1 + s2) / 2;

    return gcd(avg, sigma);
}
