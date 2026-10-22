/*
 * A314015  Coordination sequence Gal.4.100.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            a(n-6) Initial values: a(0)=1, a(1)=5, a(2)=11, a(3)=15,
 *            a(4)=19, a(5)=25, a(6)=30 Computes the coordination sequence
 *            4.100.3 (OEIS A314015)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[7] = {
    (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)15,
    (seq_t)19, (seq_t)25, (seq_t)30
};

static seq_t *cache = NULL;     /* memoization array          */
static size_t  cache_size = 0;  /* current allocated size     */
static idx_t   max_computed = (idx_t)-1; /* highest index with value */

static void ensure_cache(idx_t n)
{
    /* required size, at least up to n */
    size_t required = (size_t)n + 1;
    size_t init_len  = 7;

    /* Allocate and initialise on first call, or grow as needed */
    if (cache == NULL) {
        if (required < init_len) init_len = required;
        cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (!cache) return;                 /* allocation failure – caller will get 0 */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        cache_size   = required;
        max_computed = init_len - 1;
    } else if (required > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return;                 /* keep old cache on failure */
        cache     = tmp;
        cache_size = required;
    }

    /* If the requested index is already computed, nothing to do */
    if (max_computed >= n) return;

    /* Compute missing values using the recurrence, starting from max_computed+1 */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += cache[i-1];           /* 2*a(i-1) */
        val -= cache[i-2];
        val -= cache[i-2];           /* -2*a(i-2) */
        val += cache[i-3];
        val += cache[i-3];           /* +2*a(i-3) */
        val -= cache[i-4];
        val -= cache[i-4];           /* -2*a(i-4) */
        val += cache[i-5];
        val += cache[i-5];           /* +2*a(i-5) */
        val -= cache[i-6];           /* -a(i-6) */

        cache[i] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 7) return init[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
