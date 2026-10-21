/*
 * A269673  Number of length-n 0..3 arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo 3+1.
 * https://oeis.org/A269673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - a(n-2) - 15*a(n-3)
   with initial values a(1)=4, a(2)=16, a(3)=60. */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Return the nth term (n ≥ 1).  The function is thread‑unsafe
   but memoises values across calls so that successive calls,
   even in arbitrary order, run in O(n) time. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but safe guard */
    size_t sn = (size_t)n;        /* index into the cache */

    /* Extend the cache if necessary */
    if (sn > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
        if (new_cap < sn) new_cap = sn;      /* ensure we reach sn */

        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) {                        /* allocation failure */
            return 0;                      /* fallback: zero value */
        }
        cache = tmp;

        /* Compute new entries */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i == 1)          cache[i] = 4;
            else if (i == 2)     cache[i] = 16;
            else if (i == 3)     cache[i] = 60;
            else
                cache[i] = 5 * cache[i - 1] - cache[i - 2] - 15 * cache[i - 3];
        }
        cache_cap = new_cap;
    }
    return cache[sn];
}
