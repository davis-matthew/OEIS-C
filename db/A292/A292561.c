/*
 * A292561  Expansion of Product_{k>=1} (1 - mu(k)^2*x^k), where mu() is the
 *          Moebius function (A008683).
 * https://oeis.org/A292561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in * ∏_{k≥1, square‑free k} (1 - x^k)
 *            * = signed number of partitions of n into distinct square‑free
 *            parts * (+ for even number of parts, – for odd)
 * Range:     i8 n <= 75, i16 n <= 229, i32 n <= 738, i64 n <= 2504,
 *            i128 n <= 9080, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;      /* memoised coefficients */
static idx_t cache_size = 0;     /* current allocated size (number of slots) */

/* Return the nth term (n ≥ 0).  The function may be called in any order. */
seq_t a(idx_t n)
{
    /* If the cache is too small, recompute all terms up to n. */
    if (n + 1 > cache_size) {
        idx_t size = n + 1;                  /* number of array elements */
        seq_t *new_cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* allocation failure (unlikely) */
        cache = new_cache;

        /* initialise all entries to 0 and set a[0] = 1 */
        for (idx_t i = 0; i < size; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;

        /* classic 0‑1 knapsack update for every square‑free part k */
        for (idx_t k = 1; k < size; ++k) {
            if (is_squarefree((seq_t)k)) {
                for (idx_t j = size - 1; j >= k; --j)
                    cache[j] -= cache[j - k];
            }
        }
        cache_size = size;
    }
    return cache[n];
}
