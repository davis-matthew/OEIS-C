/*
 * A292076  Number of 4-cycles in the n-Menger sponge graph.
 * https://oeis.org/A292076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 244*a(n-2) + 480*a(n-3) Initial values:
 *            a(1)=0, a(2)=192, a(3)=5952
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* cache[1..cache_len] holds a(k), cache[0] unused */
static idx_t cache_len = 0;  /* highest index stored in cache */

/* Compute the n-th term of OEIS A292076. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1, undefined for n<1 */

    /* First call: allocate cache and store the first three values */
    if (cache == NULL) {
        cache = (seq_t *)calloc(4, sizeof(seq_t));   /* indices 0..3, 0 unused */
        if (!cache) return (seq_t)0;                 /* allocation failed */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)192;
        cache[3] = (seq_t)5952;
        cache_len = 3;
    }

    if (n <= cache_len) return cache[n];

    /* Extend the cache to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                     /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)31  * cache[i - 1]
          - (seq_t)244 * cache[i - 2]
          + (seq_t)480 * cache[i - 3];
    }
    cache_len = n;
    return cache[n];
}
