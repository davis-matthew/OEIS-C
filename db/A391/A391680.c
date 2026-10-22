/*
 * A391680  Number of integer compositions of n that are not the first sums of
 *          any composition.
 * https://oeis.org/A391680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence a(1)=1, a(2)=1, a(3)=3, a(4)=6, a(5)=13, a(6)=27
 *            a(n)=2*a(n-1)+3*a(n-2)-5*a(n-3)-4*a(n-4)+2*a(n-5)+4*a(n-6)
 *            (n>=7) Source: rational generating function of A391680
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoization array, 1‑based index */
static size_t cache_len = 0;    /* number of computed entries (index of last) */
static size_t cache_cap = 0;    /* allocated capacity, cache_cap-1 >= cache_len */

/* base values for n = 1..6 */
static const seq_t init_values[6] = { 1, 1, 3, 6, 13, 27 };

seq_t a(idx_t n)
{
    /* n is >= 1 (offset is 1).  Work with size_t for array indexing. */
    if (n < 1) return 0;  /* undefined for n<1, return 0 as placeholder */

    /* Ensure cache is allocated and contains at least the first 6 values. */
    if (cache == NULL) {
        cache_cap = (size_t)n > 6 ? (size_t)n : 6;
        cache_cap++; /* allocate one extra for 1‑based array */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 1; i <= 6; ++i)        /* cache[1]..cache[6] */
            cache[i] = init_values[i - 1];
        cache_len = 6;
    }

    /* If the requested n is already computed, return it. */
    size_t ni = (size_t)n;
    if (ni <= cache_len)
        return cache[ni];

    /* Grow the cache if necessary. */
    if (ni >= cache_cap) {
        cache_cap = ni + 1;            /* 1‑based array */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute new values up to n. */
    for (size_t i = cache_len + 1; i <= ni; ++i) {
        /* Compute: a[i] = 2*a[i-1] + 3*a[i-2] + 2*a[i-5] + 4*a[i-6]
                    - (5*a[i-3] + 4*a[i-4]);   (i>=7)  */
        seq_t pos = 0, neg = 0;
        pos += 2 * cache[i - 1];
        pos += 3 * cache[i - 2];
        pos += 2 * cache[i - 5];
        pos += 4 * cache[i - 6];
        neg += 5 * cache[i - 3];
        neg += 4 * cache[i - 4];
        cache[i] = pos - neg;
    }

    cache_len = ni;
    return cache[ni];
}
