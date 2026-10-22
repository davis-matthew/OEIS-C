/*
 * A372156  E.g.f. A(x) satisfies A(x) = exp( 2 * x * (1 + x * A(x)^(1/2)) ).
 * https://oeis.org/A372156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n! * Sum_{k=0..n} (n-k+2)^(k-1) * binomial(k, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from the general e.g.f. formula with parameters r=2, t=0, u=1, s=1) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* Fast power for seq_t, exponent is a small non‑negative integer */
static seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Sequence A372156 */
seq_t a(idx_t n)
{
    /* Offset 0 : a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    static seq_t *fact_cache = NULL;
    static idx_t fact_cap = (idx_t)0;

    /* Extend factorial cache up to n if necessary */
    if (n > fact_cap)
    {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure – should not happen */

        if (fact_cache)
        {
            for (idx_t i = (idx_t)0; i <= fact_cap; ++i)
                new_cache[i] = fact_cache[i];
        }

        new_cache[0] = (seq_t)1;
        for (idx_t i = fact_cap + (idx_t)1; i <= n; ++i)
            new_cache[i] = new_cache[i - (idx_t)1] * (seq_t)i;

        if (fact_cache)
            free(fact_cache);
        fact_cache = new_cache;
        fact_cap = n;
    }

    seq_t sum = (seq_t)0;
    /* The sum runs over k = 0..n; k = 0 term is zero for n>0, so start at 1 */
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* binomial(k, n-k) is zero when k < n-k, handled automatically by binom() */
        seq_t bin = binom(k, n - k);
        if (bin == (seq_t)0)
            continue;

        /* (n-k+2)^(k-1) */
        seq_t base  = (seq_t)(n - k + 2);
        seq_t power = pow_seq_int(base, k - (idx_t)1);

        /* (n! / k!) * power * binom(k, n-k) */
        seq_t comb_fact = fact_cache[n] / fact_cache[k];
        seq_t term = comb_fact * power * bin;

        sum += term;
    }

    /* Final result: r * sum, with r = 2 */
    return (seq_t)2 * sum;
}
