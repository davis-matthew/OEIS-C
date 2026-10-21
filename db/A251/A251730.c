/*
 * A251730  Numbers n such that the sum of the triangular numbers T(n) and
 *          T(n+1) is equal to the sum of two pentagonal numbers P(m) and
 *          P(m+1) for some m.
 * https://oeis.org/A251730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) initial values: a(1) = 10,
 *            a(2) = 152, a(3) = 2130
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero or negative indices are undefined; return 0 */
    if (n == 0) return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* number of terms stored (>=0) */
    static size_t cap = 0;         /* allocated capacity */

    /* ensure capacity for index n */
    while (cached_max < n) {
        /* grow array if needed */
        if (cached_max >= (idx_t)cap) {
            size_t new_cap = cap ? cap * 2 : 4;
            while (new_cap <= (size_t)cached_max) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* compute next term, index = cached_max + 1 (1‑based) */
        idx_t idx = cached_max + 1;
        seq_t val;
        if (idx == 1) {
            val = (seq_t)10;
        } else if (idx == 2) {
            val = (seq_t)152;
        } else if (idx == 3) {
            val = (seq_t)2130;
        } else {
            /* recurrence: a(i) = 15*a(i-1) - 15*a(i-2) + a(i-3) */
            val = (seq_t)15 * cache[idx - 2]
                - (seq_t)15 * cache[idx - 3]
                + cache[idx - 4];
        }

        cache[cached_max] = val;
        cached_max++;
    }

    return cache[n - 1];
}
