/*
 * A334932  Numbers that generate rotationally symmetrical XOR-triangles with
 *          a pattern of zero-triangles of edge length 3, some of which are
 *          clipped to result in some zero-triangles of edge length 2 at the
 *          edges.
 * https://oeis.org/A334932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 65*a(n-2) – 64*a(n-4) for n > 4 initial values:
 *            a(1)=2535, a(2)=3705, a(3)=162279, a(4)=237177
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Static cache for already computed terms. */
static seq_t *cache      = NULL;          /* array of terms (index 0 unused) */
static idx_t  cached_n   = 0;             /* largest index that has been computed */
static size_t cache_cap  = 0;             /* allocated size of cache (in elements) */

/* Ensure that the cache can hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    /* We need space for indices 0..n inclusive. */
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 8)          /* avoid too-small allocations */
        new_cap = 8;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;          /* allocation failed – caller will get 0 */
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Compute a(n) for the OEIS sequence A334932. */
seq_t a(idx_t n)
{
    if (n == 0)               /* sequence is defined for n ≥ 1; return 0 as a safe default */
        return (seq_t)0;

    /* Make sure the cache can store index n. */
    ensure_capacity(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)2535;
        else if (i == 2)
            cache[i] = (seq_t)3705;
        else if (i == 3)
            cache[i] = (seq_t)162279;
        else if (i == 4)
            cache[i] = (seq_t)237177;
        else
            cache[i] = (seq_t)65 * cache[i - 2] - (seq_t)64 * cache[i - 4];
    }

    cached_n = n;
    return cache[n];
}
