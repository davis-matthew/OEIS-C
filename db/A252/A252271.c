/*
 * A252271  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 2 3 4 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 2 3 4 6 or 7.
 * https://oeis.org/A252271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 7*a(n-6) + 2*a(n-9) for n>12 Initial values:
 *            a(1)=206, a(2)=185, a(3)=232, a(4)=322, a(5)=535, a(6)=852,
 *            a(7)=1260, a(8)=2221, a(9)=3676, a(10)=5538, a(11)=9911,
 *            a(12)=16558
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 36, u64 n <= 80, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        0,      /* dummy for index 0 */
        206, 185, 232, 322, 535, 852,
        1260, 2221, 3676, 5538, 9911, 16558
    };

    /* Static cache for memoization */
    static seq_t *cache   = NULL;   /* pointer to allocated array */
    static idx_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_size = 0;    /* next free index (one past last valid) */

    /* Handle small indices directly */
    if (n <= (idx_t)12)
        return init[(size_t)n];

    /* Grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? n + 1 : cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;          /* avoid overflow */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)                                       /* allocation failed */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cached values up to index 12 if not already done */
    if (cache_size <= 12) {
        for (idx_t i = cache_size == 0 ? 1 : cache_size; i <= 12; ++i)
            cache[i] = init[(size_t)i];
        cache_size = 13;
    }

    /* Compute all needed terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 6 * cache[i - 3] - 7 * cache[i - 6] + 2 * cache[i - 9];
    }
    cache_size = n + 1;

    return cache[(size_t)n];
}
