/*
 * A257444  Number of (n+2) X (5+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) for n > 5, with
 *            initial values a(1)=65, a(2)=68, a(3)=77, a(4)=89, a(5)=107
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms, 0‑based */
static size_t cache_len = 0;  /* number of terms currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0, return 0 just in case */

    /* Ensure the cache is allocated and large enough */
    if (cache == NULL) {
        cache_cap = 8;
        cache_len = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (cache == NULL) { /* allocation failed */
        return (seq_t)0;
    }

    /* Grow capacity if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Set initial five terms if not already present */
    if (cache_len == 0) {
        cache[0] = (seq_t)65; /* a(1) */
        cache[1] = (seq_t)68; /* a(2) */
        cache[2] = (seq_t)77; /* a(3) */
        cache[3] = (seq_t)89; /* a(4) */
        cache[4] = (seq_t)107;/* a(5) */
        cache_len = 5;
    }

    /* Compute terms up to n if necessary */
    if ((size_t)n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* i is 1‑based, array index is i-1 */
            seq_t a1 = cache[i - 2]; /* a(i-1) */
            seq_t a2 = cache[i - 3]; /* a(i-2) */
            seq_t a3 = cache[i - 4]; /* a(i-3) */
            seq_t a4 = cache[i - 5]; /* a(i-4) */
            cache[i - 1] = (seq_t)2 * a1 - a2 + a3 - a4;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
