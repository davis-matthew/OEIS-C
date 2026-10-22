/*
 * A309728  G.f. A(x) satisfies A(x) = A(x^2) / (1 - 2*x), with A(0) = 1.
 * https://oeis.org/A309728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=2*a(n-1)+((n%2==0)?a(n/2):0)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;        /* pointer to dynamic array */
static idx_t cache_size = 0;       /* number of entries stored (highest index + 1) */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Compute a(n) for any n ≥ 0.  Results are memoised in a static array. */
seq_t a(idx_t n)
{
    /* If the requested index is beyond the current cache, extend it. */
    if (n >= cache_size) {
        size_t needed = (size_t)(n + 1);

        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap *= 2;

            /* Allocate or reallocate memory. Explicit cast is required for C++. */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Compute new terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if ((i & 1) == 0) {        /* even i */
                cache[i] = (seq_t)2 * cache[i-1] + cache[i/2];
            } else {                         /* odd i */
                cache[i] = (seq_t)2 * cache[i-1];
            }
        }

        cache_size = n + 1;
    }

    return cache[n];
}
