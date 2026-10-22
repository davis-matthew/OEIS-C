/*
 * A330256  a(0) = 0; for n > 0, a(n) = n - a((Sum_{k=0..n-1} a(k)) mod n).
 * https://oeis.org/A330256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = n - a((Σ_{k=0}^{n-1} a(k)) mod n)
 * Range:     u8 n <= 28, u16 n <= 969, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of previously computed terms */
static seq_t *cache = NULL;      /* array holding a(0) .. a(max_computed) */
static idx_t  cache_cap = 0;     /* allocated capacity of cache          */
static idx_t  cache_max = 0;     /* largest index that has been computed */
static seq_t  cum_sum = 0;       /* Σ_{k=0}^{cache_max} a(k)              */

/* Compute and return the n-th term of the sequence */
seq_t a(idx_t n)
{
    /* Handle the first term */
    if (n == 0) {
        if (cache_cap == 0) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            if (!cache) abort();
            cache_cap = 1;
            cache[0] = 0;
            cum_sum = 0;
            cache_max = 0;
        }
        return 0;
    }

    /* Initialise or extend the cache if necessary */
    if (cache_cap == 0) {                 /* first call with n > 0          */
        cache_cap = n + 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = 0;
        cum_sum = 0;
        cache_max = 0;
    } else if (cache_cap <= n) {          /* grow cache to accommodate 'n' */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return a pre‑computed value if it exists */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t mod_index = cum_sum % (seq_t)i;      /* (Σ a(k)) mod i          */
        idx_t idx = (idx_t)mod_index;              /* 0 ≤ idx < i              */
        seq_t val = (seq_t)i - cache[idx];         /* a(i) = i - a(idx)       */
        cache[i] = val;
        cum_sum += val;
    }

    cache_max = n;
    return cache[n];
}
