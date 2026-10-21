/*
 * A271346  Numbers k such that the final digit of k^k is 6.
 * https://oeis.org/A271346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) for n > 7 with initial terms
 *            a(1)=4, a(2)=6, a(3)=8, a(4)=12, a(5)=14, a(6)=16, a(7)=24
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, harmless */

    /* static state for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cap      = 0;
    static idx_t last     = 0;           /* highest index computed so far */

    /* ensure capacity up to n */
    if (cap <= n) {
        idx_t old_cap = cap;
        idx_t new_cap = (cap == 0) ? (n + 1) * 2 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
        /* initialise any newly allocated slots */
        for (idx_t i = old_cap; i < cap; ++i) cache[i] = (seq_t)0;
    }

    /* initialise first 7 terms if not already */
    static const seq_t init[8] = {0, 4, 6, 8, 12, 14, 16, 24};
    if (last == 0) { /* first run: fill the known values */
        for (idx_t i = 1; i <= 7 && i <= n; ++i) cache[i] = init[i];
        last = (n < 7) ? n : 7;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(n) = a(n-1) + a(n-6) - a(n-7) */
            cache[i] = cache[i - 1] + cache[i - 6] - cache[i - 7];
        }
    }
    last = n;

    return cache[n];
}
