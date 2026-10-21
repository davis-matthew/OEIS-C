/*
 * A252864  Number of pairs in generation n of the tree T defined in Comments.
 * https://oeis.org/A252864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) for n >= 12 with initial values a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=12, a(7)=18,
 *            a(8)=25, a(9)=35, a(10)=51, a(11)=75
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Base values for n = 0 … 11 */
static const seq_t base_vals[12] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)8,
    (seq_t)12, (seq_t)18, (seq_t)25, (seq_t)35, (seq_t)51, (seq_t)75
};

/* Cached computed terms, initially empty. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Extend the cache to contain indices 0 … n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;           /* already available */

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;              /* allocation failure – abort silently */

    /* Copy previously computed values. */
    for (idx_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* Compute the remaining values. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12) {
            new_cache[i] = base_vals[i];
        } else {
            new_cache[i] = new_cache[i - 1] + new_cache[i - 3];
        }
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

/* Return the nth term of A252864. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
