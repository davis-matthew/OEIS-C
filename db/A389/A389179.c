/*
 * A389179  Number of mutual-visibility sets in the n-gear graph.
 * https://oeis.org/A389179
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a_n = 5*a_{n-1} - 6*a_{n-2} + 2*a_{n-3} - a_{n-4} + a_{n-5} for
 *            n ≥ 12 Initial terms (defined for n ≥ 3): a_3 = 78, a_4 = 190,
 *            a_5 = 504, a_6 = 1 562, a_7 = 5 142, a_8 = 17 236, a_9 = 58 126,
 *            a_10 = 196 432, a_11 = 664 294. For n < 3 the function returns 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values up to a_11 (index 0..11) */
    static const seq_t init[12] = {
        0ULL, 0ULL, 0ULL,                /* a_0, a_1, a_2 (unused) */
        78ULL,           /* a_3 */
        190ULL,          /* a_4 */
        504ULL,          /* a_5 */
        1562ULL,         /* a_6 */
        5142ULL,         /* a_7 */
        17236ULL,        /* a_8 */
        58126ULL,        /* a_9 */
        196432ULL,       /* a_10 */
        664294ULL        /* a_11 */
    };

    if (n < 3)
        return (seq_t)0;

    if (n < 12)
        return init[n];

    /* Memoisation: cache values up to the maximal n requested. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of allocated slots */

    /* Grow cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure handled as zero */
        cache = new_cache;
        cache_size = new_size;

        /* Initialise first 12 values if not already present */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
    }

    /* Compute missing values only once */
    for (idx_t i = (idx_t)12; i <= n; ++i) {
        seq_t temp_pos = (seq_t)5 * cache[i - 1]
                       + (seq_t)2 * cache[i - 3]
                       + cache[i - 5];

        seq_t temp_neg = (seq_t)6 * cache[i - 2]
                       + cache[i - 4];

        cache[i] = temp_pos - temp_neg;
    }

    return cache[n];
}
