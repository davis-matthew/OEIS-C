/*
 * A381484  Expansion of e.g.f. exp(-x/3) / (1-3*x)^(1/9).
 * https://oeis.org/A381484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0
   a(n) = (n-1) * (3*a(n-1) + a(n-2))   for n > 1 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL; /* memoisation array */
static size_t cache_cap = 0;/* current allocation size (in seq_t units) */
static idx_t cache_max = 0; /* highest index that is currently stored */

/* Compute the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap = 2;                       /* need slots for indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory: return 0 */

        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        cache[1] = (seq_t)0;                 /* a(1) = 0 */
        cache_max = 1;
    }

    /* Return the cached value if already computed. */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Grow the array if necessary. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1)) {
            new_cap <<= 1;   /* double until big enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t prev1 = cache[k - 1];
        seq_t prev2 = cache[k - 2];
        seq_t term   = (seq_t)(k - 1) * (((seq_t)3 * prev1) + prev2);
        cache[k] = term;
    }
    cache_max = n;

    return cache[n];
}
