/*
 * A388286  Möbius transform of A325973, where A325973 is the arithmetic mean
 *          of {sum of unitary divisors} and {sum of squarefree divisors}.
 * https://oeis.org/A388286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} μ(n/d) * A325973(d) (§A388286)
 * Range:     u8 n <= 255, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Compute the A325973 value for a single argument (n) */
static seq_t A325973_calc(idx_t n)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t unitary = 1;            /* product of (1 + p^e) over prime powers */
    seq_t squarefree = 1;        /* product of (1 + p) over distinct primes */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute p^e */
        seq_t power = 1;
        for (uint64_t k = 0; k < e; ++k)
            power *= (seq_t)p;

        unitary *= (seq_t)(1) + power;
        squarefree *= (seq_t)(1) + (seq_t)p;
    }

    /* arithmetic mean (sum + squarefree)/2  */
    return (unitary + squarefree) / (seq_t)2;
}

/* Compute the Mobius function μ(m) where m ≥ 1 */
static int mobius_from_n(idx_t m)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2)      /* square factor -> μ = 0 */
            return 0;
    }

    /* μ(m) = (-1)^k where k is the number of distinct prime factors */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n == 0, but safeguard */

    /* Factor n once */
    uint64_t primes[64], exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Generate all divisors of n */
    idx_t divs[512];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur = div_cnt;
        idx_t mult = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            mult *= (idx_t)p;
            for (size_t j = 0; j < cur; ++j)
                divs[div_cnt++] = divs[j] * mult;
        }
    }

    long long acc = 0;   /* using signed long long for safety */

    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        idx_t q = n / d;                     /* q = n/d */
        int mu = mobius_from_n(q);           /* μ(q) */
        seq_t val = A325973_calc(d);         /* A325973(d) */
        acc += mu * (long long)val;          /* accumulate signed contribution */
    }

    return (seq_t)acc;
}
