/*
 * A255490  The subsequence A247649(2^n-1).
 * https://oeis.org/A255490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 4*a(n-4) for n >= 4; a(0)=1, a(1)=5, a(2)=7,
 *            a(3)=19
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of elements currently stored */

    /* initialise the cache with the first four terms */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0; /* unlikely, but keep the compiler happy */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)19;
    }

    /* if we already have the requested term return it */
    if (n < (idx_t)cap) return cache[(size_t)n];

    /* grow the cache until it contains the n-th term */
    size_t i;
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        /* reallocate memory */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;  /* unlikely, but keep the compiler happy */
        cache = tmp;
        /* compute all newly available terms */
        for (i = cap; i < new_cap; ++i) {
            /* recurrence holds only for i >= 4, but any i < 4
               will be left as zero (unused) because we start from i=cap>=4. */
            cache[i] = (seq_t)3 * cache[i - 2] + (seq_t)4 * cache[i - 4];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
