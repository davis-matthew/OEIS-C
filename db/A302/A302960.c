/*
 * A302960  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) - 56*a(n-4) + 64*a(n-5) initial
 *            terms: a(1)=4, a(2)=29, a(3)=154, a(4)=833, a(5)=4527,
 *            a(6)=24602
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache for already computed terms.  index 0 of the array holds a(1) */
static seq_t *cache = NULL;
static idx_t  cache_used = 0;   /* number of terms computed and stored */
static size_t cache_cap  = 0;   /* allocated capacity of the array */

static const seq_t init_vals[6] = {
    (seq_t)4ULL,
    (seq_t)29ULL,
    (seq_t)154ULL,
    (seq_t)833ULL,
    (seq_t)4527ULL,
    (seq_t)24602ULL
};

/* ensure the cache can hold at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_used >= n) return;

    if (cache == NULL) {
        /* start with a small size */
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < (size_t)n) {
        /* grow to at least the required size (double strategy) */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the cache with initial values if needed */
    for (idx_t i = cache_used; i < n; ++i) {
        idx_t j = i + 1; /* term number (1‑based) */
        if (j <= 6)
            cache[i] = init_vals[j - 1];
        else {
            /* recurrence:
               a(n) = 7*a(n-1) - 7*a(n-2) - 56*a(n-4) + 64*a(n-5)   */
            seq_t sum1 = ((seq_t)7 * cache[i - 1])    /* 7*a(n-1) */
                       + ((seq_t)64 * cache[i - 5]);   /* + 64*a(n-5) */
            seq_t sum2 = ((seq_t)7 * cache[i - 2])    /* 7*a(n-2) */
                       + ((seq_t)56 * cache[i - 4]);   /* + 56*a(n-4) */
            cache[i] = sum1 - sum2;                  /* result of the recurrence */
        }
    }
    cache_used = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
