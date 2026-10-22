/*
 * A363116  Expansion of e.g.f. log(1 - (1/3)*log(1-3*x)).
 * https://oeis.org/A363116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0 , a(n) = 3^(n‑1)*(n‑1)! – ∑_{k=1}^{n‑1}
 *            C(n‑1,k)*(k‑1)!*3^(k‑1)*a(n‑k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches all required values up to the largest
   argument ever requested.  All multiplications use the arithmetic
   defined for seq_t, so it works for fixed‑width integer types as
   well as for the arbitrary‑precision type supplied by the build. */

#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* a[0 …] */
static seq_t *fact_cache = NULL;  /* factorial[0 …] ; factorial[0] = 1   */
static seq_t *pow3_cache  = NULL;  /* 3^i  (i≥0) */
static idx_t  cache_size  = 0;     /* next free index   */

/* Grow the caches so that they contain indices 0 … n. */
static void ensure_up_to(idx_t n)
{
    if (n < cache_size) return;

    size_t new_size = (size_t)(n + 1);
    a_cache   = (seq_t*)realloc(a_cache,   new_size * sizeof(seq_t));
    fact_cache = (seq_t*)realloc(fact_cache, new_size * sizeof(seq_t));
    pow3_cache  = (seq_t*)realloc(pow3_cache,  new_size * sizeof(seq_t));

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)    /* initialise base cases   */
        {
            a_cache[0]   = (seq_t)0;
            fact_cache[0] = (seq_t)1;
            pow3_cache[0]  = (seq_t)1;
        }
        else
        {
            /* factorial[i] = i * factorial[i‑1]  */
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;

            /* pow3[i] = 3 * pow3[i‑1]            */
            pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;

            /* compute a[i] by the recurrence   */
            seq_t bin   = (seq_t)1;          /* C(n‑1,0)    */
            seq_t sum   = (seq_t)0;          /* right–hand sum */

            for (idx_t k = 1; k < i; ++k)
            {
                /* update binomial coefficient C(i‑1,k) */
                bin *= (seq_t)(i - k);       /* multiply by (i‑k)  */
                bin /= (idx_t)k;             /* divide by k        */

                seq_t term = a_cache[i-k];   /* a(n‑k)          */
                term *= bin;                /* * binomial      */
                term *= fact_cache[k-1];    /* *(k‑1)!         */
                term *= pow3_cache[k-1];    /* *3^(k‑1)        */

                sum += term;                /* accumulate      */
            }

            /* first term: 3^(i‑1)*(i‑1)!                     */
            seq_t first = pow3_cache[i-1] * fact_cache[i-1];
            a_cache[i] = first - sum;
        }
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;
    ensure_up_to(n);
    return a_cache[n];
}
