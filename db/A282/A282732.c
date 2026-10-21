/*
 * A282732  Satisfies the recurrence a(n) =
 *          3*a(n-1)-a(n-2)+a(n-3)-2*a(n-4)+2*a(n-5).
 * https://oeis.org/A282732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-a(n-2)+a(n-3)-2*a(n-4)+2*a(n-5); a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=23, a(4)=63, a(5)=171
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for the first six indices. */
    static const seq_t base[6] = {1, 3, 9, 23, 63, 171};

    /* Static cache to keep already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (>=0) */
    static idx_t cache_cap = 0;   /* current allocation size */

    /* Directly return if the requested index is one of the first six. */
    if (n < 6) {
        return base[n];
    }

    /* Allocate or resize the cache as necessary. */
    if (cache == NULL) {
        /* Start with a capacity that covers the requested index. */
        cache_cap = n + 1;
        if (cache_cap < 6) {
            cache_cap = 6;
        }
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache_len = 0;
    } else if (cache_cap <= n) {
        /* Grow the array to hold up to index n. */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill in the initial base terms if they are not yet stored. */
    if (cache_len < 6) {
        for (idx_t i = cache_len; i < 6; ++i) {
            cache[i] = base[i];
        }
        cache_len = 6;
    }

    /* Compute any missing terms up to and including a(n). */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Apply the recurrence:
           a(i) = 3*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4) + 2*a(i-5) */
        seq_t term = 3 * cache[i - 1];
        term -= cache[i - 2];
        term += cache[i - 3];
        term -= 2 * cache[i - 4];
        term += 2 * cache[i - 5];
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[n];
}
