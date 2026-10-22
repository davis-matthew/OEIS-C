/*
 * A367720  E.g.f. satisfies A(x) = exp(x*A(x^4)).
 * https://oeis.org/A367720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 a(n) = (n-1)! * Σ_{k=0..⌊(n-1)/4⌋} (4k+1) * a(k)
 *            * a(n-1-4k) / (k! * (n-1-4k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------ */

#include <stddef.h>
#include <stdlib.h>

/* Helper to make sure an array has space for indices 0..need */
static void
ensure_cap(size_t need, size_t *cap, seq_t **arr)
{
    if (need >= *cap) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        if (new_cap <= need) new_cap = need + 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* -------------------------------------------------------------- */

static seq_t *cache      = NULL;   /* memoised a(n) values            */
static size_t cache_cap  = 0;      /* capacity of cache array         */
static size_t cache_last = 0;      /* largest index computed so far   */

static seq_t *fact      = NULL;    /* factorial values 0! … n!        */
static size_t fact_cap  = 0;       /* capacity of fact array          */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache == NULL) {
        ensure_cap(1, &cache_cap, &cache);
        cache[0] = (seq_t)1;
        cache_last = 0;
    }
    if (fact == NULL) {
        ensure_cap(1, &fact_cap, &fact);
        fact[0] = (seq_t)1;
    }

    /* Make sure arrays can hold indices up to n */
    ensure_cap((size_t)n + 1, &cache_cap, &cache);
    ensure_cap((size_t)n + 1, &fact_cap, &fact);

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)(cache_last + 1); i <= n; ++i) {
        /* factorial */
        fact[i] = fact[i - 1] * (seq_t)i;

        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        /* recurrence */
        seq_t sum = 0;
        idx_t limit = (i - 1) / 4;
        for (idx_t k = 0; k <= limit; ++k) {
            seq_t term = fact[i - 1];
            term /= fact[k];
            term /= fact[i - 1 - 4 * k];
            term *= (seq_t)(4 * k + 1);
            term *= cache[k];
            term *= cache[i - 1 - 4 * k];
            sum += term;
        }
        cache[i] = sum;
    }

    cache_last = (size_t)n;
    return cache[n];
}
