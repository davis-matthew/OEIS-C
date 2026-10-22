/*
 * A335848  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * k!! * a(n-k).
 * https://oeis.org/A335848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k!! * a(n-k); a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state shared between calls */
static seq_t *a_cache        = NULL;          /* array of computed a(i) */
static seq_t *df_cache       = NULL;          /* array of computed k!!  (double factorials) */
static size_t cache_cap      = 0;              /* allocated capacity of the arrays */
static idx_t  max_computed   = 0;              /* largest index for which a(i) has been computed */
static int    initialized    = 0;              /* flag indicating first call has been handled */

/* ensure that the arrays can index up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;          /* double until sufficient */

    if (new_cap <= cache_cap)
        return;                 /* already large enough */

    seq_t *new_a  = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_df = (seq_t *)malloc(new_cap * sizeof(seq_t));

    if (a_cache) {
        for (size_t i = 0; i < cache_cap; ++i) {
            new_a[i]  = a_cache[i];
            new_df[i] = df_cache[i];
        }
        free(a_cache);
        free(df_cache);
    }

    a_cache = new_a;
    df_cache = new_df;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise on first invocation */
    if (!initialized) {
        ensure_capacity(0);
        a_cache[0] = (seq_t)1;  /* a(0) = 1 */
        df_cache[0] = (seq_t)1; /* 0!! = 1 */
        initialized = 1;
        max_computed = 0;
    }

    /* grow the cache if necessary and compute missing values */
    if (n > max_computed) {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {

            /* compute i!! (double factorial) */
            if (i == 1)
                df_cache[1] = (seq_t)1;                /* 1!! = 1 */
            else if (i >= 2)
                df_cache[i] = (seq_t)i * df_cache[i - 2];

            /* compute a(i) by the recurrence */
            seq_t sum = (seq_t)0;
            seq_t C   = (seq_t)1;                    /* binomial(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                C = (C * (seq_t)(i - k + 1)) / (seq_t)k;   /* binomial(i,k) */
                seq_t term = C * df_cache[k];              /* binomial * k!! */
                term = term * a_cache[i - k];              /* multiply by a(i-k) */
                sum = sum + term;
            }
            a_cache[i] = sum;
        }
        max_computed = n;
    }

    return a_cache[n];
}
