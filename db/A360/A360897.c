/*
 * A360897  G.f. satisfies A(x) = 1 + x * A(x * (1 - x^3)).
 * https://oeis.org/A360897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 19, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360897
   Recurrence: a(0) = 1
   For n > 0: a(n) = Σ_{k=0..⌊(n‑1)/4⌋} (-1)^k * binomial(n‑1‑3k , k) * a(n‑1‑3k)
   (see OEIS entry for further description)
*/
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */
#include "../utils/ensure_cache.h" /* provides ensure_cache(idx_t, seq_t**, size_t*) */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    /* Ensure the cache can hold up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* If we already computed this value, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute new values from cache_len up to n inclusive */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        idx_t maxk = (i - 1) / 4;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t index = i - 1 - 3 * k;
            seq_t term = binomial(index, k);
            term = term * cache[index];
            if (k & 1) {
                sum -= term;
            } else {
                sum += term;
            }
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
