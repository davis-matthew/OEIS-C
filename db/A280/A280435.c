/*
 * A280435  Number of nX3 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5)
   for n > 6, with initial values
   a(1) = 1, a(2) = 10, a(3) = 21, a(4) = 42, a(5) = 77, a(6) = 136. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe default. */
    if (n < 1)
        return (seq_t)0;

    /* Memoization cache: cache[i] holds a(i) for 1 ≤ i < cached_len. */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;            /* number of allocated entries */

    /* target is the index we need to compute. */
    size_t target = (size_t)n;

    if (target + 1 > cached_len) {
        /* Need to extend the cache. */
        size_t new_len = target + 1;              /* includes index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                      /* allocation failure */
        cache = tmp;

        /* Compute new entries from the first missing index up to target. */
        size_t start = cached_len;
        if (start == 0) start = 1;                /* skip index 0 */

        for (size_t i = start; i <= target; ++i) {
            switch (i) {
                case 1: cache[i] = (seq_t)1;    break;
                case 2: cache[i] = (seq_t)10;   break;
                case 3: cache[i] = (seq_t)21;   break;
                case 4: cache[i] = (seq_t)42;   break;
                case 5: cache[i] = (seq_t)77;   break;
                case 6: cache[i] = (seq_t)136;  break;
                default: {
                    /* a(i) = 3*a(i-1) - a(i-2) - 3*a(i-3) + a(i-4) + a(i-5) */
                    seq_t term = (seq_t)3 * cache[i - 1];
                    term -= cache[i - 2];
                    term -= (seq_t)3 * cache[i - 3];
                    term += cache[i - 4];
                    term += cache[i - 5];
                    cache[i] = term;
                }
            }
        }

        cached_len = new_len;
    }

    return cache[(size_t)n];
}
