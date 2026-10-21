/*
 * A262062  The first of six consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of seven
 *          consecutive positive integers.
 * https://oeis.org/A262062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 27*a(n-2) + a(n-3) (n>3) initial values:
 *            a(1)=85, a(2)=2269, a(3)=58969
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* sequence starts at n=1 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity */
    static size_t len   = 0; /* largest index computed (len>=0) */

    /* determine needed storage */
    size_t needed = (size_t)n + 1;   /* cache[0] unused */
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base terms */
    if (len < 1) { cache[1] = (seq_t)85; len = 1; }
    if (len < 2) { cache[2] = (seq_t)2269; len = 2; }
    if (len < 3) { cache[3] = (seq_t)58969; len = 3; }

    /* compute new terms up to n */
    for (size_t i = len + 1; i <= n; ++i) {
        cache[i] = (seq_t)27 * cache[i - 1]
                 - (seq_t)27 * cache[i - 2]
                 + cache[i - 3];
    }
    len = n;

    return cache[n];
}
