/*
 * A271218  Number of symmetric linked diagrams with n links and no simple
 *          link.
 * https://oeis.org/A271218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (2n-3)*a(n-2) - (2n-5)*a(n-3) + 2*a(n-4) -
 *            a(n-5); Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=12
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_size = 0; /* number of terms currently stored (max index + 1) */

static void ensure_cache(idx_t n)
{
    /* First time: allocate at least the first 5 terms */
    if (cache_size == 0) {
        cache_cap = (n < 4 ? 5 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)3;   /* a(3) */
        cache[4] = (seq_t)12;  /* a(4) */
        cache_size = 5;
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms from the current cache_size up to n */
    if (n + 1 > cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Recurrence holds for i >= 5; the loop never reaches i < 5 */
            seq_t term = 2 * cache[i-1];
            term += ((seq_t)(2*i - 3)) * cache[i-2];
            term -= ((seq_t)(2*i - 5)) * cache[i-3];
            term += 2 * cache[i-4];
            term -= cache[i-5];
            cache[i] = term;
        }
        cache_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
