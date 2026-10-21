/*
 * A252527  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 8*a(n-6) for n > 8, a(1)=621, a(2)=341,
 *            a(3)=466, a(4)=610, a(5)=1114, a(6)=1748, a(7)=2332, a(8)=4244
 * Range:     u8 none, u16 n <= 14, u32 n <= 38, u64 n <= 86, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* dynamic cache of computed values */
static size_t cache_size = 0;     /* number of entries stored (max n computed) */
static size_t cache_cap  = 0;     /* capacity of the cache array */

static const seq_t init_vals[8] = {
    (seq_t)621, (seq_t)341, (seq_t)466, (seq_t)610,
    (seq_t)1114, (seq_t)1748, (seq_t)2332, (seq_t)4244
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */
    if (n <= 8) return init_vals[n-1];

    /* lazily allocate and initialize the cache */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
        cache_size = 8;
    }

    /* enlarge cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        /* recurrence uses indices k-3 and k-6 (zero‑based: k-4, k-7) */
        cache[k-1] = cache[k-4] * (seq_t)6 - cache[k-7] * (seq_t)8;
    }

    cache_size = (size_t)n;
    return cache[n-1];
}
