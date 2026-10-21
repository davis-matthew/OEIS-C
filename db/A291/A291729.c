/*
 * A291729  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - 2 S - S^2.
 * https://oeis.org/A291729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) + 2*a(n-4) + a(n-6) for n>=7
 *            Initial terms: a(0)=2, a(1)=5, a(2)=14, a(3)=39, a(4)=106,
 *            a(5)=290, a(6)=794
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {2, 5, 14, 39, 106, 290, 794};

static seq_t *cache          = NULL;   /* stored values */
static idx_t  cache_size     = 0;      /* array capacity */
static idx_t  computed       = 0;      /* highest index already computed */

static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;               /* need indices 0..n */

    /* Allocate or grow the cache */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[needed];
#else
        cache = malloc(needed * sizeof(seq_t));
#endif
        cache_size = needed;
        /* initialise first seven terms */
        for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
        computed = 6;
    }
    else if (needed > cache_size) {
#ifdef __cplusplus
        seq_t *newp = new seq_t[needed];
        for (idx_t i = 0; i < cache_size; ++i) newp[i] = cache[i];
        delete[] cache;
        cache = newp;
#else
        void *tmp = realloc(cache, needed * sizeof(seq_t));
        cache = (seq_t *)tmp;
#endif
        cache_size = needed;
    }

    /* Compute any missing terms up to n */
    if (computed < n) {
        idx_t start = computed + 1;
        if (start < 7) start = 7;        /* first computed term is a(7) */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] =
                (seq_t)(2) * cache[i-1] + cache[i-2] +
                (seq_t)(2) * cache[i-3] + (seq_t)(2) * cache[i-4] +
                cache[i-6];
        }
        computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 7) return init_vals[n];
    ensure_cache(n);
    return cache[n];
}
