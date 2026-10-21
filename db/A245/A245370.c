/*
 * A245370  Number of compositions of n into parts 3, 5 and 9.
 * https://oeis.org/A245370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-9) with a(0) = 1, a(k) = 0 for k <
 *            0
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    /* handle n == 0 separately */
    if (n == 0) {
        if (cap == 0) {
            cap  = 1;
            cache = (seq_t *)malloc(sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* ensure the cache can hold indices up to n */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? 4 : cap << 1;   /* double or start with 4 */
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                    /* allocation failed */
        /* initialise new slots to 0 */
        for (idx_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* make sure a(0) is set */
    if (cap > 0)
        cache[0] = (seq_t)1;

    /* compute values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 3) val += cache[i - 3];
        if (i >= 5) val += cache[i - 5];
        if (i >= 9) val += cache[i - 9];
        cache[i] = val;
    }

    return cache[n];
}
