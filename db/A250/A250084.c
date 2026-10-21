/*
 * A250084  Number of length 3+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>8, with initial values:
 *            a(1)=144, a(2)=2228, a(3)=15472, a(4)=68863, a(5)=232096,
 *            a(6)=647122, a(7)=1572320, a(8)=3441309
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* cache[0] is unused; cache[1]..cache[cache_size] hold a(1)..a(cache_size) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* offset handling */
    if (n < 1) return (seq_t)0;

    /* initialize cache with the first 8 terms */
    if (cache_size == 0) {
        cache_size = 8;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)0;                  /* unused */
        cache[1] = (seq_t)144;
        cache[2] = (seq_t)2228;
        cache[3] = (seq_t)15472;
        cache[4] = (seq_t)68863;
        cache[5] = (seq_t)232096;
        cache[6] = (seq_t)647122;
        cache[7] = (seq_t)1572320;
        cache[8] = (seq_t)3441309;
    }

    /* return cached value if already computed */
    if (n <= cache_size) return cache[n];

    /* grow cache to accommodate up to n */
    size_t new_size = (size_t)n + 1; /* indices 0..n */
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;            /* allocation failure */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t term = (seq_t)8   * cache[k-1]
                   - (seq_t)28  * cache[k-2]
                   + (seq_t)56  * cache[k-3]
                   - (seq_t)70  * cache[k-4]
                   + (seq_t)56  * cache[k-5]
                   - (seq_t)28  * cache[k-6]
                   + (seq_t)8   * cache[k-7]
                   - cache[k-8];
        cache[k] = term;
    }
    cache_size = n;
    return cache[n];
}
