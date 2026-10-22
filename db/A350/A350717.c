/*
 * A350717  a(n) = 4*a(n-1) - n - 1, for n > 0, a(0) = 1.
 * https://oeis.org/A350717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - n - 1, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms          */
static idx_t   cache_len = 0;     /* number of terms actually stored  */
static size_t  cache_cap = 0;     /* allocated capacity in array      */

/* Grow the cache array to hold at least `needed` elements.           */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term of the sequence A350717.                       */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache_capacity(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                /* a(i) = 4*a(i-1) - i - 1   */
                cache[i] = cache[i-1] * (seq_t)4 - (seq_t)(i + 1);
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
