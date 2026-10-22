/*
 * A337823  a(n) = prime(n-1) - floor(a(n-1)/2); a(1)=1.
 * https://oeis.org/A337823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-1) - floor(a(n-1)/2); a(1) = 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1
    a(n) = prime(n-1) - floor(a(n-1)/2)   for n ≥ 2
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Compute the n-th term of OEIS A337823.
   The function is memoised: the cache is built on demand and
   can be queried in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */

    /* Initialise: a(1) = 1 in cache[0] */
    if (cache == NULL) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                         /* a(1) */
        cache_size = 1;
    }

    /* Return value if already computed */
    if ((idx_t)cache_size >= n) {
        return cache[n - 1];
    }

    /* Extend cache up to term n */
    size_t old_size = cache_size;
    cache_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failure – propagate failure sentinel */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing terms iteratively */
    for (idx_t k = old_size + 1; k <= n; ++k) {
        seq_t prev = cache[k - 2];                    /* a(k‑1) */
        seq_t prime_val = (seq_t)get_prime(k - 1);    /* prime(k‑1) */
        seq_t half_prev = prev / (seq_t)2;            /* floor(a(k‑1)/2) */
        cache[k - 1] = prime_val - half_prev;
    }

    return cache[n - 1];
}
