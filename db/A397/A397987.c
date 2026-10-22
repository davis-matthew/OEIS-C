/*
 * A397987  Upper (1/2,1/3) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) - a(n-6) +
 *            2*a(n-7) - a(n-8) + a(n-9) - 2*a(n-10) + a(n-11) (a(0)=0,
 *            a(1)=1, a(2)=3, a(3)=6, a(4)=11, a(5)=16, a(6)=23, a(7)=31,
 *            a(8)=40, a(9)=50, a(10)=61)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;       /* array of computed terms       */
static size_t cache_size = 0;     /* number of elements currently stored (max index + 1) */
static size_t cache_cap = 0;      /* allocated capacity              */

static const seq_t init[11] = {
    0, 1, 3, 6, 11, 16, 23, 31, 40, 50, 61
};

/* Ensure the cache has capacity at least 'need' */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute the upper (1/2,1/3)-midsequence of triangular numbers and squares */
seq_t a(idx_t n)
{
    /* Return initial values without caching for n < 11 */
    if (n < 11)
        return init[n];

    /* First call: initialise cache with the first 11 terms */
    if (cache == NULL) {
        grow_cache(11);
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_size = 11;
    }

    /* If already computed up to n, return stored value */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Compute missing terms up to n */
    grow_cache((size_t)n + 1);
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = 0;
        /* Sum positive contributions first */
        val += 2 * cache[i - 1];
        val += cache[i - 3];
        val += cache[i - 5];
        val += 2 * cache[i - 7];
        val += cache[i - 9];
        val += cache[i - 11];
        /* Subtract negative contributions */
        val -= cache[i - 2];
        val -= 2 * cache[i - 4];
        val -= cache[i - 6];
        val -= cache[i - 8];
        val -= 2 * cache[i - 10];
        cache[i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
