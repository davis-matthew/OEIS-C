/*
 * A323715  a(n) = Product_{k=0..n} (2^k + 3^k).
 * https://oeis.org/A323715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0..n} (2^k + 3^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;

/* Return the n-th term of the sequence a(n) = ∏_{k=0..n} (2^k + 3^k). */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough. */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Determine from which index to start computing. */
    idx_t start = 0;
    if (last_computed != (idx_t)-1) {
        start = last_computed + 1;
    }

    /* Compute missing terms and store them in the cache. */
    for (idx_t i = start; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 2;                 /* 2^0 + 3^0 = 2 */
        } else {
            seq_t term = pow2(i) + pow3(i);
            cache[i] = cache[i - 1] * term;
        }
    }

    last_computed = n;
    return cache[n];
}
