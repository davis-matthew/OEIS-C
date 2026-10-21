/*
 * A293579  Number of compositions of n where each part i is marked with a
 *          word of length i over a binary alphabet whose letters appear in
 *          alphabetical order and both letters occur at least once in the
 *          composition.
 * https://oeis.org/A293579
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) initial values: a(2)=3,
 *            a(3)=16, a(4)=66
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute OEIS sequence A293579 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        /* undefined for n < 2; return 0 */
        return (seq_t)0;
    }

    /* static cache: cache[i] holds a(i) for i >= 2 */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 4;   /* highest index already computed */

    /* we need indexes up to at least n */
    if (n + 1 > cap) {
        size_t newcap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) {
            /* allocation failed; return 0 to signal error */
            return (seq_t)0;
        }
        /* zero new entries */
        for (size_t i = cap; i < newcap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* initialize first three values if not already */
    if (!cache[2]) cache[2] = (seq_t)3;
    if (!cache[3]) cache[3] = (seq_t)16;
    if (!cache[4]) cache[4] = (seq_t)66;

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* recurrence: a(i) = 6*a(i-1) - 10*a(i-2) + 4*a(i-3) */
            cache[i] = (seq_t)6 * cache[i-1]
                     - (seq_t)10 * cache[i-2]
                     + (seq_t)4 * cache[i-3];
        }
        last = n;
    }

    return cache[n];
}
