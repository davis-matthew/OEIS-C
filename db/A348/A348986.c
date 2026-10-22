/*
 * A348986  Denominator of ratio sigma(n) / A325973(n), where A325973 is the
 *          arithmetic mean of {sum of squarefree divisors} and {sum of
 *          unitary divisors}.
 * https://oeis.org/A348986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A325973(n) / gcd(A000203(n), A325973(n)) (denominator of
 *            σ(n) / A325973(n))
 * Range:     u8 n <= 16, u16 n <= 3754, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum of squarefree divisors: prod (1 + p) */
    seq_t S_sf = (seq_t)1;
    /* Sum of unitary divisors: prod (1 + p^e) */
    seq_t S_unit = (seq_t)1;
    /* σ(n) = prod (p^{e+1} - 1) / (p - 1) */
    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* p^e */
        seq_t powp = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j) powp *= p;

        /* Update sums */
        S_sf = S_sf * ((seq_t)1 + p);           /* S_sf *= (1 + p) */
        S_unit = S_unit * ((seq_t)1 + powp);    /* S_unit *= (1 + p^e) */

        /* σ term for this prime */
        seq_t numerator   = powp * p - (seq_t)1;   /* p^{e+1} - 1 */
        seq_t denominator = p - (seq_t)1;          /* p - 1 */
        seq_t term = numerator / denominator;
        sigma = sigma * term;
    }

    /* A325973(n) = (S_sf + S_unit) / 2 */
    seq_t A = (S_sf + S_unit) / (seq_t)2;

    /* Denominator of σ(n)/A325973(n) */
    seq_t g = gcd(sigma, A);
    return A / g;
}
