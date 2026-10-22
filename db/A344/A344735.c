/*
 * A344735  a(0) = 1; a(n) = 4 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A344735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=1..n} binomial(n,k) * a(k‑1) with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for previously computed values. */
static seq_t *cache    = NULL;   /* array of a(i) */
static idx_t cache_len = 0;      /* next free index (size of cache) */

seq_t a(idx_t n)
{
    /* Initialise cache to contain a(0). */
    if (cache_len == 0) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to hold indices up to n. */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
    if (!new_cache) return 0;            /* allocation failure */

    /* Copy previously computed values. */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms from cache_len through n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                    /* C(i,0) = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* C(i,k) = C(i,k‑1) * (i–k+1) / k  (exact division) */
            bin = bin * (seq_t)(i - k + 1);
            bin = bin / (seq_t)k;
            sum += bin * cache[k - 1];
        }

        new_cache[i] = sum * 4;           /* a(i) = 4 * sum */
    }

    /* Replace old cache with the new one. */
    free(cache);
    cache      = new_cache;
    cache_len  = new_len;

    return cache[n];
}
