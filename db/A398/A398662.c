/*
 * A398662  Upper (1/2, 1) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6), with a(0)=0, a(1)=2, a(2)=7, a(3)=17, a(4)=35, a(5)=63
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values up to a(5) */
static const seq_t init_vals[6] = {0, 2, 7, 17, 35, 63};

/* cache for computed terms */
static seq_t *cache = NULL;   /* array of terms */
static idx_t cache_len = 0;   /* number of valid entries in cache */

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* small indices use the fixed array */
    if (n < 6)
        return init_vals[n];

    /* allocate cache if necessary and ensure space up to n */
    if (cache == NULL) {                         /* first call */
        size_t need = n + 1;                     /* size in elements */
        if (need < 6) need = 6;                 /* at least six */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) return 0;                    /* allocation failed */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;                          /* we know indices 0..5 */
    } else if (cache_len <= n) {                 /* must grow */
        size_t need = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed */
        cache = tmp;
        /* cache_len remains unchanged; we will fill missing terms below */
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* use a wider type to avoid intermediate overflow */
        __int128 v = 0;
        v += (__int128)cache[i - 1] * 4;
        v -= (__int128)cache[i - 2] * 7;
        v += (__int128)cache[i - 3] * 8;
        v -= (__int128)cache[i - 4] * 7;
        v += (__int128)cache[i - 5] * 4;
        v -= (__int128)cache[i - 6];
        cache[i] = (seq_t)v;
    }
    cache_len = n + 1;                           /* now we have all up to n */
    return cache[n];
}
