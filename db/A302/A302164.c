/*
 * A302164  Number of 2Xn 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 16*a(n-4) - 8*a(n-5) (n > 6) a(1)=2, a(2)=3,
 *            a(3)=9, a(4)=17, a(5)=25, a(6)=65
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* 1‑based cache of computed terms   */
static idx_t cache_size  = 0;      /* highest index currently cached    */

static const seq_t init_vals[7] = {
    (seq_t)0,   /* index 0 unused */
    (seq_t)2,   /* a(1) */
    (seq_t)3,   /* a(2) */
    (seq_t)9,   /* a(3) */
    (seq_t)17,  /* a(4) */
    (seq_t)25,  /* a(5) */
    (seq_t)65   /* a(6) */
};

/* Expand cache to contain terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return; /* already computed */

    /* Allocate new array of size n+1 (ignore index 0) */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure – fallback to 0 */

    /* Copy existing values, if any */
    if (cache) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* Initialise missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 6)
            new_cache[i] = init_vals[(size_t)i];
        else
            new_cache[i] =
                new_cache[i - 1]
              + (seq_t)16 * new_cache[i - 4]
              - (seq_t)8  * new_cache[i - 5];
    }

    cache       = new_cache;
    cache_size  = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, return 0 */

    ensure_cache(n);
    return cache[n];
}
