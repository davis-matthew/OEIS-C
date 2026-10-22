/*
 * A362569  E.g.f. satisfies A(x) = exp(x/A(x)^(x^2)).
 * https://oeis.org/A362569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k * * (n-2*k)^k *
 *            binomial(n-2*k-1, k)/(n-2*k)!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=1, a(2)=1, a(3)=1  */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Auxiliary: integer power for seq_t */
static seq_t int_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Pre‑compute factorials up to n in a small array. */
    seq_t *fact = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!fact) return 0;                  /* allocation failure – return 0 */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t sum = 0;
    idx_t maxk = n / 3;
    seq_t fact_n = fact[n];

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n2k = n - 2 * k;
        /* term = n! / (n-2k)! */
        seq_t term = fact_n / fact[n2k];

        /* multiply by (n-2k)^k */
        seq_t pow_term = int_pow_seq((seq_t)n2k, k);
        term = term * pow_term;

        /* multiply by binomial(n-2k-1, k) */
        if (k == 0) {
            /* binomial(...,0) == 1, nothing to do */
        } else {
            term = term * binomial(n2k - 1, k);
        }

        /* apply sign */
        if (k & 1)
            term = -term;

        sum = sum + term;
    }

    free(fact);
    return sum;
}
