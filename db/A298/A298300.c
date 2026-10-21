/*
 * A298300  Analog of Motzkin numbers for Coxeter type D.
 * https://oeis.org/A298300
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies 2*n*a(n)+(-7*n+6)*a(n-1)+9*(n-4)*a(n-3)=0 with
 *            a(2)=1, a(3)=4, a(4)=11
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0; /* last index whose value is known */

/* compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0; /* undefined, safe sentinel */

    /* ensure the cache can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base cases on the first call */
    if (cache_last == 0) {
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)11;
        cache_last = 4;
    }

    /* return previously computed value */
    if (n <= cache_last)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* use 128‑bit intermediates to avoid overflow when seq_t is 64‑bit */
        unsigned __int128 num =
            (unsigned __int128)(7 * i - 6) * (unsigned __int128)cache[i - 1]
            - (unsigned __int128)9 * (unsigned __int128)(i - 4) * (unsigned __int128)cache[i - 3];

        cache[i] = (seq_t)(num / (unsigned __int128)(2 * i));
    }
    cache_last = n;
    return cache[n];
}
