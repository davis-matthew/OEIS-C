/*
 * A361283  Expansion of e.g.f. exp(x/(1-x)^4).
 * https://oeis.org/A361283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361283: Expansion of e.g.f. exp(x/(1-x)^4).
   Recurrence (valid for n ≥ 5):
   a(n) = (5n-4) * a(n-1)
          - (n-1)(10n-23) * a(n-2)
          + 10(n-1)(n-2)(n-3) * a(n-3)
          - 5(n-1)(n-2)(n-3)(n-4) * a(n-4)
          + (n-5)(n-1)(n-2)(n-3)(n-4) * a(n-5);
   Initial values:
   a(0)=1, a(1)=1, a(2)=9, a(3)=85, a(4)=961.  */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;   /* number of elements currently allocated      */
static idx_t  cache_last = 0;  /* largest index that has been computed */

/* ensure the cache holds at least (n+1) entries */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        size_t new_size = new_cap * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 5;                         /* at least space for the 5 bases */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)85;
        cache[4] = (seq_t)961;
        cache_last = 4;
    }

    /* if n already computed, return immediately */
    if (n <= cache_last)
        return cache[n];

    /* grow the cache if needed */
    ensure_cache(n);

    /* compute successive terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 4) /* base case already set */
            continue;

        /* helper scalars */
        seq_t c1 = (seq_t)(5 * i - 4);    /* (5n-4)                    */
        seq_t c2 = (seq_t)(i - 1);        /* (n-1)                     */
        seq_t c3 = (seq_t)(10 * i - 23);  /* (10n-23)                  */
        seq_t c4 = (seq_t)(i - 2);        /* (n-2)                     */
        seq_t c5 = (seq_t)(i - 3);        /* (n-3)                     */
        seq_t c6 = (seq_t)(i - 4);        /* (n-4)                     */
        seq_t c7 = (seq_t)(i - 5);        /* (n-5)                     */

        /* build the terms of the recurrence */
        seq_t t1 = c1 * cache[i - 1];                                    /* (5n-4) * a(n-1) */
        seq_t t2 = (c2 * c3) * cache[i - 2];                            /* (n-1)(10n-23) * a(n-2) */
        seq_t t3 = (((seq_t)10 * c2) * c4) * c5 * cache[i - 3];          /* 10(n-1)(n-2)(n-3) * a(n-3) */
        seq_t t4 = (((((seq_t)5 * c2) * c4) * c5) * c6 * cache[i - 4]); /* 5(n-1)(n-2)(n-3)(n-4) * a(n-4) */
        seq_t t5 = ((c7 * c2) * c4) * c5 * c6 * cache[i - 5];            /* (n-5)(n-1)(n-2)(n-3)(n-4) * a(n-5) */

        seq_t val = t1;
        val -= t2;
        val += t3;
        val -= t4;
        val += t5;

        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
