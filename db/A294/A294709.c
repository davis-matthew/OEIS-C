/*
 * A294709  Number of permutations of [n] avoiding {2143, 3412, 1234}.
 * https://oeis.org/A294709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) with a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=69
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 146, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static size_t cache_len = 0;  /* allocated length of the cache */

static const seq_t init_vals[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)6, (seq_t)21, (seq_t)69 };

static void grow_cache_to(idx_t n)
{
    /* Ensure cache has space for terms 0 .. n inclusive */
    size_t needed = (size_t)n + 1;
    if (cache_len >= needed) return;

    size_t new_len = cache_len ? cache_len << 1 : 64;
    if (new_len < needed) new_len = needed;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_len];
#else
    new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;   /* silently abort if malloc fails - caller will handle */
#endif

    /* Copy existing elements */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* Initialise the first known values if still missing */
    if (cache_len < 6) {
        for (size_t i = cache_len; i < 6; ++i) new_cache[i] = init_vals[i];
    }

    /* free old cache */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 6) return init_vals[n];

    grow_cache_to(n);

    /* Compute missing terms up to n */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        /* recurrence calculation using only arithmetic on seq_t */
        seq_t val = cache[i-1];
        val = val * (seq_t)6;
        val = val - (cache[i-2] * (seq_t)15);
        val = val + (cache[i-3] * (seq_t)20);
        val = val - (cache[i-4] * (seq_t)15);
        val = val + (cache[i-5] * (seq_t)6);
        val = val - cache[i-6];
        cache[i] = val;
    }

    return cache[n];
}
