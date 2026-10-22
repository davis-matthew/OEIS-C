/*
 * A373905  a(n) = Sum_{k=0..floor(n/3)} binomial(n+3*k,n-3*k).
 * https://oeis.org/A373905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373905
 *  Formula:  a(n) = 6*a(n-1) - 15*a(n-2) + 21*a(n-3)
 *            - 15*a(n-4) + 6*a(n-5) - a(n-6)
 *  Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=8, a(5)=29
 *  (Same as a(n) = Σ_{k=0..⌊n/3⌋} binomial(n+3k, n-3k))
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t max_n   = 5;   /* last index stored in cache */
    static bool initialized = false;

    /* First call: allocate and initialize cache with known terms */
    if (!initialized) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return 0;   /* allocation failure – unlikely */
        cache[0] = 1; cache[1] = 1; cache[2] = 1;
        cache[3] = 2; cache[4] = 8; cache[5] = 29;
        initialized = true;
    }

    /* n within known range – return directly */
    if (n <= max_n)
        return cache[(size_t)n];

    /* Grow cache to accommodate up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp) return 0;            /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t v = 6   * cache[(size_t)(i - 1)]
               - 15  * cache[(size_t)(i - 2)]
               + 21  * cache[(size_t)(i - 3)]
               - 15  * cache[(size_t)(i - 4)]
               + 6   * cache[(size_t)(i - 5)]
               -      cache[(size_t)(i - 6)];
        cache[(size_t)i] = v;
    }

    max_n = n;
    return cache[(size_t)n];
}
