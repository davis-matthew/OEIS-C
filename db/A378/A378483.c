/*
 * A378483  Expansion of (Sum_{k>=0} binomial(3*k,k) * x^k)^3.
 * https://oeis.org/A378483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (27*a(n-1) + 3*binomial(3*n, n)) / 4 (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A378483: expansion of (∑ binomial(3k,k) x^k)^3 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *seq_cache = NULL;   /* cached values a[0..len-1] */
static size_t cache_len = 0;      /* current number of stored elements */

/* enlarge cache to hold indices up to 'need' (inclusive) */
static void ensure_cache(idx_t need)
{
    if ((need + 1) <= cache_len) return;

    size_t new_cap = (size_t)(need + 1);
    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) return;                 /* allocation failed – caller will see zero */

    /* copy old data */
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = seq_cache[i];

    /* initialise new slots to zero – they'll be overwritten */
    for (size_t i = cache_len; i < new_cap; ++i)
        new_buf[i] = 0;

    free(seq_cache);
    seq_cache = new_buf;
    cache_len = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise singleton cache if necessary */
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;   /* allocation failure */
        seq_cache[0] = 1;                 /* a(0) */
        cache_len  = 1;
    }

    /* if n already cached, return immediately */
    if ((size_t)n < cache_len)
        return seq_cache[(size_t)n];

    /* compute all missing terms up to n */
    size_t old_len = cache_len;          /* previous cache size */
    ensure_cache(n);                     /* now cache_len >= n+1 */

    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            seq_cache[0] = 1;
            continue;
        }
        idx_t idx = (idx_t)i;
        seq_t bin = binomial(3 * idx, idx);          /* C(3*idx, idx) */
        seq_t numer = seq_cache[i - 1] * 27 + bin * 3;
        seq_cache[i] = numer / 4;                     /* integer division is exact */
    }

    return seq_cache[(size_t)n];
}
