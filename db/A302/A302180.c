/*
 * A302180  Number of 3D walks of type aad.
 * https://oeis.org/A302180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=7; for n≥4 (n+4)*(n+2)*a(n) =
 *            3*(n^2+3n+1)*a(n-1) +13*(n-1)*(n+1)*a(n-2)
 *            -15*(n-1)*(n-2)*a(n-3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory management */
#include <stdlib.h>

/* The types seq_t and idx_t are defined in the build environment.  */

/* Memoisation cache */
static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_cap = 0;         /* current capacity of the cache    */
static size_t cache_size = 0;        /* number of terms stored (index+1) */

/* Ensure the cache is large enough to hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        /* Double the capacity until we can hold n+1 elements. */
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* Out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the first four known values. */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialised */
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    /* a(0) … a(3) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)7;
    cache_size = 4;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    init_cache();                      /* make sure cache exists */

    if (n < cache_size) return cache[n];

    ensure_cache(n);                   /* extend cache to hold n */

    /* Compute new terms up to n.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) {                   /* safety: already set */
            continue;
        }
        seq_t i64 = (seq_t)i;
        /* numerator = 3*(i^2+3i+1)*a(i-1) + 13*(i-1)*(i+1)*a(i-2)
           - 15*(i-1)*(i-2)*a(i-3) */
        seq_t num = (seq_t)0;
        seq_t term1 = (seq_t)3 *
                      ((seq_t)i64 * i64 + (seq_t)3 * i64 + (seq_t)1) *
                      cache[i-1];
        seq_t term2 = (seq_t)13 *
                      (i64 - (seq_t)1) * (i64 + (seq_t)1) *
                      cache[i-2];
        seq_t term3 = (seq_t)15 *
                      (i64 - (seq_t)1) * (i64 - (seq_t)2) *
                      cache[i-3];
        num = term1 + term2;
        num = num - term3;

        /* denominator = (i+4)*(i+2) */
        seq_t denom = (i64 + (seq_t)4) * (i64 + (seq_t)2);

        cache[i] = num / denom;       /* integer division is exact */
    }
    cache_size = n + 1;
    return cache[n];
}
