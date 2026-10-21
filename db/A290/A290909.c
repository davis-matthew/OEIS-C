/*
 * A290909  p-INVERT of the positive integers, where p(S) = 1 - 5*S^2.
 * https://oeis.org/A290909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 4*a(n-3) - a(n-4) with a(0) = 0, a(1)
 *            = 5, a(2) = 20, a(3) = 75
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* computed terms */
    static idx_t  cached = 0;    /* number of entries in cache */
    static idx_t  capacity = 0;  /* allocated size of cache */

    /* Base cases for n < 4 */
    if (n < 4) {
        static const seq_t base[4] = {0, 5, 20, 75};
        return base[n];
    }

    /* Make sure cache has space for indices up to n */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort gracefully */
            return 0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise cache with the first 4 terms if it hasn't been done */
    if (cached == 0) {
        cache[0] = 0;
        cache[1] = 5;
        cache[2] = 20;
        cache[3] = 75;
        cached = 4;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t val;
        if (i < 4) {             /* this case is not expected because i>=cached>=4 */
            switch (i) {
                case 0: val = 0; break;
                case 1: val = 5; break;
                case 2: val = 20; break;
                case 3: val = 75; break;
            }
        } else {
            val = 4 * cache[i - 1] - cache[i - 2] + 4 * cache[i - 3] - cache[i - 4];
        }
        cache[i] = val;
    }
    cached = n + 1;
    return cache[n];
}
