/*
 * A382641  a(n) = round(c^n), where c is the supergolden ratio A092526.
 * https://oeis.org/A382641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, a(5)=7, a(6)=10, a(7)=15, a(8)=21 (relevant for
 *            n >= 9)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* memoized sequence values */
static idx_t  cache_len = 0;  /* number of values stored in cache */

/* Ensure the cache contains values for all indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Determine new buffer size: double current size or at least n+1 */
    idx_t new_len = cache_len ? cache_len * 2 : 9;
    if (new_len <= n)      /* grow enough to hold index n */
        new_len = n + 1;

    seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (tmp == NULL) { /* Allocation failure – abort sequence generation */
        abort();
    }

    /* Copy existing cached values (if any) */
    if (cache_len > 0)
        memcpy(tmp, cache, cache_len * sizeof(seq_t));

    /* Initialize the first 9 terms if this is the first allocation */
    if (cache_len == 0) {
        const seq_t init[9] = {1, 1, 2, 3, 5, 7, 10, 15, 21};
        idx_t limit = new_len < 9 ? new_len : 9;
        for (idx_t i = 0; i < limit; ++i)
            tmp[i] = init[i];
    }

    /* Compute terms from the point where the recurrence can be applied */
    idx_t start = (cache_len == 0) ? 9 : cache_len;
    for (idx_t i = start; i < new_len; ++i)
        tmp[i] = tmp[i - 1] + tmp[i - 3];

    /* Free old cache and update pointers */
    if (cache)
        free(cache);
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
