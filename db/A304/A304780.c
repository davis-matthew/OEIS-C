/*
 * A304780  Consider a triangle whose first row is {1,2} and, for n > 1, has
 *          as its n-th row the integers k through 2k where k is the sum of
 *          the numbers in the (n-1)th row. Then a(n) is the first number in
 *          the n-th row.
 * https://oeis.org/A304780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = (3*a(n-1)*(a(n-1)+1))/2 for n>1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache   = NULL;   /* array of computed terms (0‑based, a(1)=cache[0]) */
static size_t cache_len = 0;    /* number of terms currently stored (max index computed + 1) */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* grow the cache to at least 'need' elements, doubling capacity each time */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) { /* out of memory – abort (each build has a main that handles this) */
        abort();
    }
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for 0; return 0 to satisfy any caller */
        return (seq_t)0;

    /* ensure we have space for n terms */
    grow_cache((size_t)n);

    /* compute any missing terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i == 0) {                 /* a(1) */
            cache[0] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t tmp  = prev * (prev + (seq_t)1);
            tmp = tmp * (seq_t)3;
            tmp = tmp / (seq_t)2;     /* exact division by 2 */
            cache[i] = tmp;
        }
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
