/*
 * A368438  Expansion of e.g.f. exp(x) / (3 - 2*exp(2*x)).
 * https://oeis.org/A368438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A368438: Expansion of e.g.f. exp(x) / (3 - 2*exp(2*x))
   Recurrence: a(0) = 1,
   a(n) = 1 + 2 * Sum_{k=1..n} 2^k * binomial(n,k) * a(n-k)
   Computes the term for any n ≥ 0, memoising results.
*/
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute the nth term of OEIS sequence A368438. */
seq_t a(idx_t n)
{
    /* Resize cache if needed. */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* Out of memory */
        cache = tmp;
        /* Initialise newly allocated slots to zero. */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Base case (first time). */
    if (cache[0] == 0) cache[0] = 1;

    /* Compute all values up to n that are still unknown. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0) continue;   /* already known */

        seq_t sum = 0;
        /* Compute binomial(i,k) and 2^k incrementally. */
        seq_t binom = i;        /* binomial(i,1) = i */
        seq_t pow2_val = 2;     /* 2^1 */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* term = binom * a(i-k) * 2^k */
            seq_t term = binom;
            term = term * cache[i - k];
            term = term * pow2_val;
            sum += term;

            /* Prepare for next k: binom = binomial(i,k+1),
               pow2_val = 2 * pow2_val. */
            if (k < i)
            {
                /* binom = binom * (i - k) / (k + 1) */
                seq_t num = binom * (i - k);
                seq_t den = k + 1;
                binom = num / den;   /* exact division */
                pow2_val = pow2_val * 2;
            }
        }

        cache[i] = 1 + (seq_t)2 * sum;
    }

    return cache[n];
}
