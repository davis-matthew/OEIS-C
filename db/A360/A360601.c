/*
 * A360601  E.g.f. satisfies A(x) = exp(x*A(x)^2) / (1-x).
 * https://oeis.org/A360601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360601: a(n) = n!*∑_{k=0..n} (2k+1)^(k-1)·binomial(n+k,n−k)/k! */
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* RETURN a single term of OEIS sequence A360601. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* n!  (fact_n) */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum = (seq_t)0;

    /* ratio = n!/k!  (currently k = 0) */
    seq_t ratio = fact_n;

    /* binomial = binomial(n+k,n−k)  (k = 0) */
    seq_t binom = (seq_t)1;

    /* (2k+1)^(k-1)  (k = 0 => 1) */
    seq_t pow_k = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = ratio * pow_k * binom */
        seq_t term = ratio;
        term = term * pow_k;
        term = term * binom;
        sum += term;

        if (k == n)
            break;

        /* advance ratio to n!/(k+1)! */
        ratio /= (seq_t)(k + 1);

        /* update binomial:
           binom_{k+1} = binom_k * (n+k+1)*(n−k) / ((2k+1)*(2k+2)) */
        seq_t num1 = (seq_t)(n + k + 1);
        seq_t num2 = (seq_t)(n - k);
        seq_t den1 = (seq_t)(2 * k + 1);
        seq_t den2 = (seq_t)(2 * k + 2);
        binom = binom * num1 * num2 / (den1 * den2);

        /* compute next power (2*(k+1)+1)^(k) */
        seq_t base = (seq_t)(2 * k + 3);
        seq_t exp  = (seq_t)(k);            /* (k+1)−1 = k */
        pow_k = big_pow(base, exp);
    }

    return sum;
}
