/*
 * A282703  a(n) = 3*a(n-1) + a(n-2), with a(0)=7, a(1)=26.
 * https://oeis.org/A282703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) with a(0)=7, a(1)=26
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cap   = 0;      /* allocated capacity (in terms) */
    static size_t size  = 0;      /* number of terms stored (valid indices [0..size-1]) */

    /* Ensure the cache can hold at least n+1 elements */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        if (newcap < n + 1) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – caller will see 0 */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base cases if not already done */
    if (size == 0) {
        cache[0] = (seq_t)7;
        size = 1;
    }
    if (size == 1) {
        cache[1] = (seq_t)26;
        size = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] + cache[i-2];
    }
    size = n + 1;

    return cache[n];
}
