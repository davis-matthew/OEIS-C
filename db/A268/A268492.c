/*
 * A268492  Orbit of 2 under the map A268488: n -> least number k of the form
 *          k = n*(last digit of k) + (k without its last digit).
 * https://oeis.org/A268492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268492: orbit of 2 under the map
 * a(0) = 2;
 * a(n) = 10*a(n-1)-1;
 * if the result is divisible by 9 divide by 9, else if divisible by 3 divide by 3.
 * This follows the recurrence described in the OEIS entry. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of stored terms */

    /* initialise cache with a(0) = 2 */
    if (size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;           /* out of memory; unlikely in tests */
        cache[0] = 2;
        size = 1;
    }

    /* if requested index already computed, return it */
    if (n < size)
        return cache[n];

    /* extend cache to hold terms up to n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;                   /* out of memory */
    cache = tmp;

    /* compute missing entries */
    for (idx_t i = size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cur  = prev * 10 - 1;        /* 10*a(n-1)-1 */

        if (cur % 9 == 0)
            cur /= 9;
        else if (cur % 3 == 0)
            cur /= 3;

        cache[i] = cur;
    }

    size = n + 1;
    return cache[n];
}
