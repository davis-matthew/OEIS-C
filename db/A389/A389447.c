/*
 * A389447  Numerator of ratio sigma(n) / A057723(n), where A057723 is sum of
 *          positive divisors of n that are divisible by every prime that
 *          divides n and sigma is the sum of divisors function.
 * https://oeis.org/A389447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sigma(n) / A057723(n) in reduced form. Where
 *            sigma(n) = sum_{d|n} d, and A057723(n) = sum of divisors of n
 *            that are divisible by every prime dividing n. The function
 *            computes σ(n)/A057723(n) = ∏ (p^{e+1}−1) / (p·(p^e−1)) over the
 *            prime factorization p^e of n, reduces the resul...
 * Range:     u8 n <= 16, u16 n <= 573, u32 n <= 9791, u64 n <= 9791,
 *            u128 n <= 9791, big any n
 * Helpers:   factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n = 1 is a trivial case: the ratio is 1/1. */
    if (n == 1) return (seq_t)1;

    /* Factor n.  The utility returns prime factors in order in `pr`,
       their exponents in `ex`, and the count in `cnt`.  64 primes is
       plenty for 64‑bit n. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t N_total = (seq_t)1;   /* total numerator */
    seq_t D_total = (seq_t)1;   /* total denominator */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = pr[i];
        size_t   e   = (size_t)ex[i];

        seq_t pseq          = (seq_t)p;
        seq_t p_pow_e       = seq_pow(pseq, (idx_t)e);           /* p^e   */
        seq_t p_pow_e_plus1 = seq_pow(pseq, (idx_t)(e + 1));     /* p^{e+1} */

        /* Individual fraction: (p^{e+1}-1) / (p*(p^e-1)) */
        seq_t N_i = p_pow_e_plus1 - (seq_t)1;                    /* numerator part */
        seq_t D_i = pseq * (p_pow_e - (seq_t)1);                /* denominator part */

        /* Accumulate the product. */
        N_total = N_total * N_i;
        D_total = D_total * D_i;
    }

    /* Reduce the fraction. */
    seq_t g = gcd(N_total, D_total);
    return N_total / g;
}
