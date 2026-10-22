/*
 * A329301  a(0) = a(1) = a(2) = 1; thereafter a(n) = a(n-1) - (-1)^n*a(n-2) +
 *          2*a(n-3).
 * https://oeis.org/A329301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = a(2) = 1; a(n) = a(n-1) - a(n-2) + 2*a(n-3) (n >=
 *            3)
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms    */
static size_t cache_cap = 0;         /* allocated capacity                 */
static idx_t  computed = 0;          /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* initialize base case values if this is the first call */
    if (cache == NULL) {
        cache_cap = 8;                     /* start with a small capacity   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failed               */
        cache[0] = 1; cache[1] = 1; cache[2] = 1;
        computed = 2;                       /* highest known value            */
    }

    /* if we already have the requested term, just return it */
    if (n <= computed) return cache[n];

    /* grow the cache if needed */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute forward from the next unknown term up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        cache[k] = cache[k-1] - cache[k-2] + ((seq_t)2 * cache[k-3]);
    }
    computed = n;
    return cache[n];
}
