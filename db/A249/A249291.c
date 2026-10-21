/*
 * A249291  Number of length 1+3 0..n arrays with no four consecutive terms
 *          having the sum of any three elements equal to three times the
 *          fourth.
 * https://oeis.org/A249291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-5) + 2*a(n-6) -
 *            3*a(n-7) + a(n-8) (offset 1) Initial values: a(1)=14, a(2)=66,
 *            a(3)=204, a(4)=524, a(5)=1098, a(6)=2070, a(7)=3584, a(8)=5808
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of A249291 */
seq_t a(idx_t n) {
    /* Handle sub‑offset values safely */
    if (n == 0) return (seq_t)0;

    /* Static cache of computed values, 1‑indexed: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */

    /* Initialize cache with first eight terms on first call */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)14;
        cache[1] = (seq_t)66;
        cache[2] = (seq_t)204;
        cache[3] = (seq_t)524;
        cache[4] = (seq_t)1098;
        cache[5] = (seq_t)2070;
        cache[6] = (seq_t)3584;
        cache[7] = (seq_t)5808;
    }

    /* Grow cache up to requested n */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) {
            /* Allocation failure: raise error or abort */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
            /* idx is the desired term number (1‑based) */
            seq_t term =   3 * cache[idx - 2]
                         - 2 * cache[idx - 3]
                         -   cache[idx - 4]
                         +   cache[idx - 6]
                         + 2 * cache[idx - 7]
                         - 3 * cache[idx - 8]
                         +   cache[idx - 9];
            cache[idx - 1] = term;
        }

        cache_size = new_size;
    }

    return cache[n - 1];
}
