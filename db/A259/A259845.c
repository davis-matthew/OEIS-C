/*
 * A259845  a(0)=1, a(1)=3, and the INVERT transform of the sequence deletes
 *          the 3.
 * https://oeis.org/A259845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(0) = 1, a(1) = 3 For n ≥ 2: a(n) = Σ_{k=0}^{n-1} a(k)·a(n-1-k)
 *            – 2·a(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises previously computed terms in a dynamically
   resizable array.  The recurrence is verified by the
   generating‑function relation for the sequence. */

#include <stdlib.h>

static seq_t *cache       = NULL;   /* memoised values */
static size_t  cache_len  = 0;      /* number of stored values */
static size_t  cache_cap  = 0;      /* allocated capacity */

/* Compute a(n) ---------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Grow cache storage if required */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure, unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any terms up to n that are still missing */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)3;
        } else {
            seq_t sum = 0;
            /* convolution part: Σ_{k=0}^{i-1} a(k)·a(i-1-k) */
            for (idx_t k = 0; k <= i-1; ++k) {
                sum += cache[k] * cache[i - 1 - k];
            }
            cache[i] = sum - ((seq_t)2 * cache[i - 2]);
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
