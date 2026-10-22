/*
 * A372234  E.g.f. A(x) satisfies A(x) = exp( 2 * x * (1 + x^2 * A(x)^(1/2))
 *          ).
 * https://oeis.org/A372234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n! * Σ_{k=0..⌊n/3⌋} (k+2)^(n-2k-1) / (k! (n-3k)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Cache factorials up to the largest n seen so far. */
    static seq_t *fact = NULL;     /* array of factorials */
    static size_t fact_cap = 0;    /* number of elements stored */

    /* Enlarge the factorial cache if necessary. */
    if (fact_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!new_fact)
            return (seq_t)0; /* out‑of‑memory sentinel */

        /* If this is the first allocation, initialise fact[0] = 1. */
        if (fact == NULL) {
            new_fact[0] = (seq_t)1;
            fact_cap = 1;
        }

        /* Compute any factorials that are missing. */
        for (size_t i = fact_cap; i <= (size_t)n; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;

        fact = new_fact;
        fact_cap = new_cap;
    }

    seq_t fact_n = fact[n];
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = n - 2 * k - 1;                      /* exponent ≥ 0 for n ≥ 1 */
        seq_t pow_term = big_pow((seq_t)(k + 2), (seq_t)exp);

        /* multinomial coefficient: n! / (k! (n-3k)!) */
        seq_t term_coef = fact_n / fact[k];
        term_coef = term_coef / fact[n - 3 * k];

        seq_t term = term_coef * pow_term;
        sum += term;
    }

    return sum * (seq_t)2;
}
