/*
 * A377954  a(n) = n! * Sum_{k=0..n} binomial(k+2,n-k) / k!.
 * https://oeis.org/A377954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(n-4)*a(n-1)+3*(n-1)*a(n-2)+2*(n-1)*(n-2)*a(n-3) for
 *            n>2, with a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation cache */
static size_t cache_len = 0;         /* number of allocated slots */

seq_t a(idx_t n)
{
    /* base cases (no memoisation required) */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 9;

    /* ensure the cache can hold up to index n */
    size_t required = (size_t)n + 1;
    size_t old_len = cache_len;

    if (cache == NULL) {
        cache = (seq_t*)malloc(required * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 9;
        cache_len = required;
    } else if (required > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = required;
    }

    /* compute missing values */
    idx_t start = 3;
    if (old_len > 3) start = (idx_t)old_len;  /* old_len is the next free index */

    for (idx_t i = start; i <= n; ++i) {
        int coeff3 = 3 * (int)(i - 1);                      /* 3*(i-1)   */
        int coeff2 = 2 * (int)(i - 1) * (int)(i - 2);        /* 2*(i-1)*(i-2) */
        seq_t term2 = cache[i-2] * (seq_t)coeff3;
        seq_t term3 = cache[i-3] * (seq_t)coeff2;

        int offset = (int)i - 4;
        seq_t term1 = cache[i-1] * (seq_t)(offset >= 0 ? offset : -offset);

        if (offset >= 0)
            cache[i] = term2 + term3 - term1;
        else
            cache[i] = term2 + term3 + term1;
    }

    return cache[n];
}
