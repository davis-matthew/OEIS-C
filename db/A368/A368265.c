/*
 * A368265  Expansion of e.g.f. exp(2*x) / (1 - x*exp(x)).
 * https://oeis.org/A368265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (n-k+2)^k / k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer power for arbitrary‑precision seq_t.
   Computes base^exp using repeated squaring. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache          = NULL;
    static size_t cache_alloc   = 0;   /* allocation size */
    static size_t computed_until = 0;  /* number of computed entries */

    /* static factorial table */
    static seq_t *fact          = NULL;
    static size_t fact_alloc    = 0;

    /* enlarge factorial table if needed */
    if ((size_t)n + 1u > fact_alloc)
    {
        size_t new_fact = (size_t)n + 1u;
        fact = (seq_t *)realloc(fact, new_fact * sizeof(seq_t));
        if (!fact)
            return (seq_t)0;   /* allocation failed */
        if (fact_alloc == 0)
        {
            fact[0] = (seq_t)1;
            fact_alloc = 1;
        }
        for (size_t j = fact_alloc; j <= (size_t)n; ++j)
        {
            fact[j] = fact[j - 1] * (seq_t)j;
        }
        fact_alloc = new_fact;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1u > cache_alloc)
    {
        size_t new_cache = (size_t)n + 1u;
        cache = (seq_t *)realloc(cache, new_cache * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed */
        cache_alloc = new_cache;
    }

    /* compute missing terms */
    for (idx_t i = (idx_t)computed_until; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        seq_t ratio = fact[(size_t)i];                /* n! / 0!   */
        for (idx_t k = (idx_t)0; k <= i; ++k)
        {
            seq_t pow_val = pow_seq((seq_t)(i - k + 2), k);
            sum += ratio * pow_val;
            if (k < i)
                ratio /= (seq_t)(k + 1);            /* n! / k! → n! / (k+1)! */
        }
        cache[(size_t)i] = sum;
    }
    computed_until = n + 1u;

    return cache[(size_t)n];
}
