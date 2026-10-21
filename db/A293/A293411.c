/*
 * A293411  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 2, a(2) =
 *          3, a(3) = 4.
 * https://oeis.org/A293411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-3) + a(n-4),
 * a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 4
 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamic cache for computed terms */
static idx_t cache_cap = 0;     /* allocated capacity (in terms)      */
static idx_t cache_len = 0;     /* number of terms actually stored     */

/* Ensure the cache has space for at least n+1 terms */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= n + 1) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();              /* unrecoverable failure   */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialize the cache with the first four terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache_len = 4;
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* initialize once */
    if (cache == NULL) init_cache();

    /* If the term is already cached, return it */
    if (n < cache_len) return cache[n];

    /* Make sure the cache can hold the new term */
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i - 1];   /* a(i-1) always exists */
        if (i >= 3) val += cache[i - 3];
        if (i >= 4) val += cache[i - 4];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
