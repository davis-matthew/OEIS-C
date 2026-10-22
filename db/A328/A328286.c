/*
 * A328286  Expansion of e.g.f. -log(1 - x - x^2/2).
 * https://oeis.org/A328286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 2; for n>=3, a(n) = ((n-1) * ((n-2) * a(n-2) +
 *            2 * a(n-1))) / 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is derived from the e.g.f. -log(1 - x - x^2/2).  */

#include <stddef.h>              // for size_t
#include "../utils/grow_seq.h"   // grow_seq function

static seq_t *cache = NULL;       // cache[0] holds a(1), cache[1] holds a(2)
static size_t cache_cap = 0;      // current capacity of cache
static idx_t cache_len = 0;       // number of terms stored (always >=0)

seq_t a(idx_t n)
{
    /*  Sequence is defined only for n >= 1 (offset 1).  */
    if (n < 1)
        return (seq_t)0;                // undefined for n < 1 – safe return

    /*  Initialise cache with the first two terms if it is empty.  */
    if (cache_len == 0) {
        /* Ensure capacity for at least 2 terms.  */
        grow_seq(&cache, &cache_cap, (size_t)2);
        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)2;            /* a(2) */
        cache_len = 2;
    }

    /*  If the requested term is already cached, return it directly. */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /*  Grow the cache to accommodate up to term n.  */
    if ((size_t)cache_cap < (size_t)n)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /*  Compute terms from cache_len+1 up to n using the recurrence. */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        /*  Cache indices: cache[0] = a(1), cache[1] = a(2), ...  */
        idx_t idx   = cur - 1;           /* index in cache for a(cur)   */
        seq_t a_nm1 = cache[idx - 1];    /* a(cur-1)                   */
        seq_t a_nm2 = cache[idx - 2];    /* a(cur-2)                   */

        /*  a(cur) = ((cur-1) * ((cur-2) * a(cur-2) + 2 * a(cur-1))) / 2 */
        seq_t term = ((seq_t)(cur - 1) *
                      (((seq_t)(cur - 2) * a_nm2) + ((seq_t)2 * a_nm1))) / (seq_t)2;

        cache[idx] = term;
    }

    cache_len = n;
    return cache[n - 1];
}
