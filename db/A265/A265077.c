/*
 * A265077  Coordination sequence for (3,6,8) tiling of hyperbolic plane.
 * https://oeis.org/A265077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)+a(n-5)-a(n-6) for n>6; a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=11, a(4)=20, a(5)=37, a(6)=66
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 37, u64 n <= 75, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache_ptr = NULL;
static size_t cache_capacity = 0;
static size_t cache_computed = 0;
static bool cache_initialized = false;

static void ensure_capacity(idx_t n)
{
    if (cache_capacity >= (size_t)n) return;

    /* allocate a new array of at least n elements */
    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[n];
#else
    tmp = (seq_t *)malloc(n * sizeof(seq_t));
#endif
    if (!tmp) exit(1);              /* abort on allocation failure */

    /* copy already computed values to the new array */
    if (cache_ptr) {
        for (size_t i = 0; i < cache_computed; ++i) tmp[i] = cache_ptr[i];
#ifdef __cplusplus
        delete [] cache_ptr;
#else
        free(cache_ptr);
#endif
    }
    cache_ptr = tmp;
    cache_capacity = (size_t)n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* initialise first seven terms on first call */
    if (!cache_initialized) {
        ensure_capacity(7);
        cache_ptr[0] = (seq_t)1;
        cache_ptr[1] = (seq_t)3;
        cache_ptr[2] = (seq_t)6;
        cache_ptr[3] = (seq_t)11;
        cache_ptr[4] = (seq_t)20;
        cache_ptr[5] = (seq_t)37;
        cache_ptr[6] = (seq_t)66;
        cache_computed = 7;
        cache_initialized = true;
    }

    /* If n is already known, return it immediately */
    if ((size_t)n < cache_computed) return cache_ptr[n];

    /* Compute all terms up to n (inclusive) */
    ensure_capacity(n + 1);
    for (idx_t i = (idx_t)cache_computed; i <= n; ++i) {
        cache_ptr[i] = cache_ptr[i-1] + cache_ptr[i-2] + 
                       cache_ptr[i-4] + cache_ptr[i-5] - cache_ptr[i-6];
    }
    cache_computed = (size_t)(n + 1);
    return cache_ptr[n];
}
