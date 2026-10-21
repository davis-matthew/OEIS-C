/*
 * A280221  a(n) = (-2)^(n-1)*a(n-1) + a(n-2) with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A280221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-2)^(n-1)*a(n-1) + a(n-2) with a(0)=0, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build environment.
   idx_t is an unsigned integer type (e.g. uint64_t).            */

#include <stdlib.h>

/* cache storage for memoisation ------------------------------------- */
static seq_t *cache      = NULL;   /* dynamically allocated array      */
static size_t cache_len  = 0;      /* highest cached index + 1        */
static size_t cache_cap  = 0;      /* allocated capacity               */
static seq_t  cache_pow;          /* (-2)^(cache_len-1) for next step */

/* ensure that capacity is at least 'need' entries ------------------- */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = need;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute all terms up to index n (inclusive) ----------------------- */
static void ensure_cache(idx_t n)
{
    /* lazy initialisation of the cache */
    if (cache_len == 0) {
        ensure_capacity(2);            /* at least a(0) and a(1) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
        cache_pow = (seq_t)-2;         /* ready for a(2)            */
    }

    if ((size_t)n < cache_len) return; /* already available */

    /* extend capacity if needed */
    ensure_capacity((size_t)n + 1);

    /* fill missing values ------------------------------------------ */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* cache_pow currently equals (-2)^(i-1)                     */
        cache[i] = cache_pow * cache[i - 1] + cache[i - 2];
        cache_pow = cache_pow * (seq_t)-2;  /* advance power */
    }
    cache_len = (size_t)n + 1;
}

/* public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];
    ensure_cache(n);
    return cache[(size_t)n];
}
