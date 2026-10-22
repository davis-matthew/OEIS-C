/*
 * A309971  a(1) = 1, a(2) = 2; a(n) = round((n-a(n-1))/a(n-2)) for n > 2.
 * https://oeis.org/A309971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = round((n - a(n-1)) / a(n-2)) with a(1)=1, a(2)=2, n>2
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_iterative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a memoized recurrence, falling back to on‑the‑fly calculation
   for indices beyond a predefined cache limit.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_iterative.h"

#define CACHE_LIMIT 20000   /* sufficient for typical use (n≈10 000) */

static seq_t cache[CACHE_LIMIT + 1];   /* 1‑based cache, cache[0] unused      */
static idx_t max_computed = 0;         /* highest index already computed   */

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the sequence, kept for safety */

    /* Return cached value if already available */
    if (n <= max_computed) return cache[n];

    /* If we are inside the cache limit, extend the cache */
    if (n <= CACHE_LIMIT) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 2;
            } else {
                seq_t num = (seq_t)i - cache[i - 1];
                seq_t den = cache[i - 2];
                cache[i] = (num + den / 2) / den;   /* round to nearest */
            }
        }
        max_computed = n;
        return cache[n];
    }

    /* For indices beyond the cache, compute on the fly without storing all
       intermediate values. */
    return compute_iterative(n);
}
