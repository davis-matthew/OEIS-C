/*
 * A275363  a(1)=3, a(2)=6, a(3)=3; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-1-a(n-2)).
 * https://oeis.org/A275363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=6, a(3)=3;
   a(n) = 2*a(n-3) - a(n-6)   for n > 6          */
#include <stdlib.h>          /* for malloc, realloc, abort */
#include "../utils/ensure_cache.h"  /* ensure_cache(idx_t n, seq_t **, size_t *) */

static seq_t *cache = NULL;   /* stores sequence values */
static size_t cache_cap = 0;  /* current capacity of cache */
static idx_t  max_computed = 0; /* largest n for which cache[n-1] is valid */

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for n=0; safety */
    /* guarantee cache large enough */
    ensure_cache(n, &cache, &cache_cap);

    /* compute missing terms up to n */
    static const seq_t init[6] = {3, 6, 3, 3, 9, 6};
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i - 1] = init[i - 1];
        } else {
            /* i-4 == i-3 (zero‑based), i-7 == i-6 (zero‑based) */
            cache[i - 1] = 2 * cache[i - 4] - cache[i - 7];
        }
    }
    max_computed = n;
    return cache[n - 1];
}
