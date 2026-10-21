/*
 * A276160  A recurrence of order 3 : a(0)=a(1)=a(2)=1 ; a(n) = (a(n-1)^2 +
 *          a(n-2)^2 + a(n-1) + a(n-2) + 1)/a(n-3).
 * https://oeis.org/A276160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276160: a(0)=a(1)=a(2)=1; a(n)=7*a(n-1)*a(n-2)-a(n-3)-1 for n≥3 */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 2;

/* Return the nth term of A276160. */
seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    /* If we have a cache and n is already computed, return it. */
    if (cache && n <= cache_last)
        return cache[n];

    /* Ensure the cache is large enough. */
    idx_t needed = n + 1;                     /* number of elements required */
    if (!cache) {
        cache = seq_alloc(needed);
        cache_cap = (size_t)needed;
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_last = 2;
    } else if ((size_t)needed > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)needed);
        cache_cap = (size_t)needed;
    }

    /* Compute terms up to n, if needed. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];

        seq_t val = (seq_t)7;
        val = val * a_nm1;
        val = val * a_nm2;
        val = val - a_nm3;
        val = val - (seq_t)1;

        cache[i] = val;
    }
    cache_last = n;

    return cache[n];
}
