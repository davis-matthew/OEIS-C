/*
 * A372160  E.g.f. A(x) satisfies A(x) = exp( 2 * x / (1 - x * A(x)^(1/2)) ).
 * https://oeis.org/A372160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * Σ_{k=0..n} (n‑k+2)^(k‑1) * binom(n‑1, n‑k) / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              (with binom(m, r)=0 if r<0 or r>m)                              */
/* This implementation directly evaluates the sum using only integer         */
/* arithmetic.  It uses the provided seq_pow utility for exponentiation.      */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* factorial n! */
    seq_t fac_n = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fac_n *= (seq_t)i;

    /* 2 * n!  (the outer factor r=2) */
    seq_t fac_n2 = fac_n * (seq_t)2;

    seq_t sum = (seq_t)0;

    /* binom(n-1, 0) = 1  and fact_k = 0! = 1  at the start */
    seq_t binom = (seq_t)1;
    seq_t fact_k = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* base = n - k + 2  */
        seq_t base = (seq_t)(n - k + 2);

        /* power = base^(k-1)  (k-1 may be 0) */
        seq_t pow_term;
        if (k == 1)
            pow_term = (seq_t)1;
        else
            pow_term = seq_pow(base, k - 1);

        /* Update binomial coefficient C(n-1, k-1) from previous step */
        if (k > 1) {
            seq_t num = (seq_t)(n - k + 1);            /* numerator */
            seq_t den = (seq_t)(k - 1);                /* denominator */
            binom = binom * num / den;
        }

        /* Update factorial k! */
        fact_k *= (seq_t)k;

        /* factor = (2 * n!) / k! */
        seq_t factor = fac_n2 / fact_k;

        /* term = binom * pow_term * factor */
        seq_t term = binom * pow_term * factor;

        sum += term;
    }

    return sum;
}
