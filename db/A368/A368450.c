/*
 * A368450  Expansion of e.g.f. exp(x) / (1 + log(1 - 3*x)/3).
 * https://oeis.org/A368450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} 3^(k-1) * (k-1)! * C(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure that *p has capacity for at least n+1 elements */
static void
ensure_capacity(idx_t n, seq_t **p, size_t *cap)
{
    size_t new_cap = (size_t)n + 1;
    if (*cap >= new_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *p = tmp;
    *cap = new_cap;
}

/* Sequence function */
seq_t
a(idx_t n)
{
    /* Cache for a(n) */
    static seq_t *a_cache = NULL;
    static size_t a_cap   = 0;
    static idx_t  a_max   = (idx_t)-1;

    /* Cache for c(k) = 3^(k-1) * (k-1)! */
    static seq_t *c_cache = NULL;
    static size_t c_cap   = 0;
    static idx_t  c_max   = 0;

    /* Incremental helpers for c(k) */
    static seq_t pow3_val = 1;   /* value of 3^(k-1) for the next k */
    static seq_t fact_val = 1;   /* value of (k-1)! for the next k */
    static idx_t  c_next   = 2;   /* next k to compute (>=2 after init) */

    /* First-time initialization */
    if (a_max == (idx_t)-1) {
        a_cache = NULL;
        a_cap   = 0;
        a_max   = 0;

        c_cache = NULL;
        c_cap   = 0;
        c_max   = 1;

        /* Allocate for a(0) */
        ensure_capacity(0, &a_cache, &a_cap);
        a_cache[0] = (seq_t)1;

        /* Allocate for c(1) = 1 */
        ensure_capacity(1, &c_cache, &c_cap);
        c_cache[1] = (seq_t)1;
    }

    /* If the requested n is beyond what we have computed, extend caches */
    if (n > a_max) {
        /* Ensure c_cache has size up to n */
        ensure_capacity(n, &c_cache, &c_cap);

        /* Compute c(k) for k = c_next .. n */
        for (idx_t k = c_next; k <= n; ++k) {
            /* Update power and factorial for new k */
            pow3_val *= (seq_t)3;
            fact_val *= (seq_t)(k - 1);
            c_cache[k] = pow3_val * fact_val;
        }
        c_next = n + 1;
        c_max = n;

        /* Ensure a_cache has size up to n */
        ensure_capacity(n, &a_cache, &a_cap);

        /* Compute a(i) for i = a_max+1 .. n */
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t sum = 0;         /* accumulator for the sum */
            seq_t bin = 1;         /* current binomial coefficient C(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* Update binomial coefficient iteratively */
                bin = bin * (seq_t)(i - k + 1);
                bin /= (seq_t)k;

                /* term = c(k) * bin * a(i-k) */
                seq_t term = c_cache[k];
                term = term * bin;
                term = term * a_cache[i - k];
                sum += term;
            }
            a_cache[i] = (seq_t)1 + sum;
        }
        a_max = n;
    }

    return a_cache[n];
}
