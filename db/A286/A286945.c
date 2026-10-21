/*
 * A286945  Number of maximal matchings in the ladder graph P_2 X P_n.
 * https://oeis.org/A286945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-4) + a(n-5) for n > 5; with a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=11, a(5)=24
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored terms  a(1) ... a(cache_size) */
static idx_t cache_size = 0;         /* number of terms already computed */
static idx_t cache_cap  = 0;         /* allocated capacity for cache */

/* Ensure that terms up to index n (1‑based) are available in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)     /* already available */
        return;

    /* Guarantee at least 5 terms are allocated to hold the base values. */
    idx_t need = (n > 5) ? n : 5;
    if (need > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 5;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms once. */
    if (cache_size == 0) {
        cache[0] = 1;      /* a(1) */
        cache[1] = 2;      /* a(2) */
        cache[2] = 5;      /* a(3) */
        cache[3] = 11;     /* a(4) */
        cache[4] = 24;     /* a(5) */
        cache_size = 5;    /* we have computed the first five terms */
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i == n-1 corresponds to a(n).  Recurrence uses a(n-1), a(n-4), a(n-5). */
        cache[i] = 2 * cache[i - 1] + cache[i - 4] + cache[i - 5];
    }

    /* cache_size now reflects the largest computed term. */
    if (n > cache_size)
        cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined below offset */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
