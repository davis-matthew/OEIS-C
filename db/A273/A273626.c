/*
 * A273626  A fourth-order divisibility sequence: a(n) = (1/14)*(Pell(4*n) +
 *          Pell(2*n)).
 * https://oeis.org/A273626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/14*(Pell(4*n)+Pell(2*n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 40*a(n-1) - 206*a(n-2) + 40*a(n-3) - a(n-4)  (n>4) */
/* Initial values: a(1)=1, a(2)=30, a(3)=995, a(4)=33660 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly (fast path). */
    if (n == 0) return 0;            /* offset guard; not used in OEIS */
    if (n <= 4) {
        static const seq_t init[5] = {0, 1, 30, 995, 33660};
        return init[n];
    }

    /* Static cache of already computed terms, 1‑based index. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* current maximum n cached */

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)5 * sizeof(seq_t));  /* indices 0..4 */
        cache_len = 4;
        cache[1] = 1;
        cache[2] = 30;
        cache[3] = 995;
        cache[4] = 33660;
    }

    /* Extend the cache if needed. */
    if (n > cache_len) {
        cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            /* Compute a(i) using the 4‑term recurrence.         */
            /* Compute additions first to avoid underflow.       */
            seq_t t1 = 40 * cache[i - 1];
            t1 += 40 * cache[i - 3];
            seq_t t2 = 206 * cache[i - 2];
            t2 += cache[i - 4];
            cache[i] = t1 - t2;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
