/*
 * A251345  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock summing
 *          to 3 4 or 5.
 * https://oeis.org/A251345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) -27*a(n-2) -173*a(n-3) +362*a(n-4) -61*a(n-5)
 *            -144*a(n-6) +48*a(n-7) Initial values: a(1)=309, a(2)=3921,
 *            a(3)=50831, a(4)=663307, a(5)=8671957, a(6)=113445117,
 *            a(7)=1484345363
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached = 0;    /* highest index already computed */
    /* ensure cache exists */
    if (!cache) {
        /* reserve space for indices 0..7 at minimum */
        idx_t size = (n > 7) ? n + 1 : 8;
        cache = (seq_t*)calloc(size, sizeof(seq_t));
        /* initialise the first 7 terms (index 0 unused) */
        cache[0] = 0;
        cache[1] = (seq_t)309;
        cache[2] = (seq_t)3921;
        cache[3] = (seq_t)50831;
        cache[4] = (seq_t)663307;
        cache[5] = (seq_t)8671957;
        cache[6] = (seq_t)113445117;
        cache[7] = (seq_t)1484345363;
        cached = 7;
    }
    /* grow cache if needed */
    if (n > cached) {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        /* compute new terms */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* recurrence */
            cache[i] = 16 * cache[i-1]
                     - 27 * cache[i-2]
                     - 173 * cache[i-3]
                     + 362 * cache[i-4]
                     - 61 * cache[i-5]
                     - 144 * cache[i-6]
                     + 48 * cache[i-7];
        }
        cached = n;
    }
    return cache[n];
}
