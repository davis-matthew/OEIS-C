/*
 * A262424  Number of (5+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          column not divisible by 3, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) +711*a(n-2) -29862*a(n-3) -141183*a(n-4)
 *            +5929686*a(n-5) +10349613*a(n-6) -434683746*a(n-7)
 *            -267400116*a(n-8) +11230804872*a(n-9) +1666598976*a(n-10)
 *            -69997156992*a(n-11) Initial values: a(1)=42, a(2)=144,
 *            a(3)=10206, a(4)=200880, a(5)=9401742, a(6)=326005344,
 *            a(7)=14036526126, a...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Initialize cache with the first 11 terms */
    if (cache_size == 0) {
        cache_cap  = 16;
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        /* a(1) … a(11) */
        cache[0]  = 42ULL;
        cache[1]  = 144ULL;
        cache[2]  = 10206ULL;
        cache[3]  = 200880ULL;
        cache[4]  = 9401742ULL;
        cache[5]  = 326005344ULL;
        cache[6]  = 14036526126ULL;
        cache[7]  = 562188062160ULL;
        cache[8]  = 23753751161742ULL;
        cache[9]  = 985963486014144ULL;
        cache[10] = 41471657300369646ULL;
        cache_size = 11;
    }

    if (n <= cache_size)
        return cache[n-1];

    /* Extend cache capacity if needed */
    if (n > cache_cap) {
        cache_cap = n * 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t offset = i - 1; /* zero‑based index for cache */

        /* Separate positive and negative contributions to avoid unsigned underflow */
        seq_t pos = 0, neg = 0;

        /* Positive terms */
        pos += (seq_t)42ULL            * cache[offset-1]; /* a(n-1) */
        pos += (seq_t)711ULL           * cache[offset-2]; /* a(n-2) */
        pos += (seq_t)5929686ULL       * cache[offset-5]; /* a(n-5) */
        pos += (seq_t)10349613ULL      * cache[offset-6]; /* a(n-6) */
        pos += (seq_t)11230804872ULL   * cache[offset-9]; /* a(n-9) */
        pos += (seq_t)1666598976ULL    * cache[offset-10];/* a(n-10) */

        /* Negative terms */
        neg += (seq_t)29862ULL         * cache[offset-3]; /* a(n-3) */
        neg += (seq_t)141183ULL        * cache[offset-4]; /* a(n-4) */
        neg += (seq_t)434683746ULL     * cache[offset-7]; /* a(n-7) */
        neg += (seq_t)267400116ULL     * cache[offset-8]; /* a(n-8) */
        neg += (seq_t)69997156992ULL   * cache[offset-11];/* a(n-11) */

        cache[offset] = pos - neg; /* final value, guaranteed non‑negative */
    }

    cache_size = n;
    return cache[n-1];
}
