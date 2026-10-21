/*
 * A250223  Number of length n+1 0..2 arrays with the sum of the cubes of
 *          adjacent differences multiplied by some arrangement of +-1 equal
 *          to zero.
 * https://oeis.org/A250223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 15*a(n-1) - 98*a(n-2) + 366*a(n-3) - 861*a(n-4) 
 *            + 1323*a(n-5) - 1328*a(n-6) + 840*a(n-7) - 304*a(n-8) + 48*a(n-9)
 * Initial values:
 *   a(1)=3, a(2)=11, a(3)=27, a(4)=79, a(5)=223,
 *   a(6)=651, a(7)=1907, a(8)=5639, a(9)=16967,
 *   a(10)=52131, a(11)=163291, a(12)=518687
 *
 * The recurrence holds for n>12.  The function caches computed values
 * so it can be called with indices in any order.
 */

#include <stdlib.h>
#include <stdint.h>

/* base values for n = 1 … 12 */
static const seq_t init[12] = {
    (seq_t)3,   (seq_t)11,  (seq_t)27,  (seq_t)79,
    (seq_t)223, (seq_t)651, (seq_t)1907, (seq_t)5639,
    (seq_t)16967, (seq_t)52131, (seq_t)163291, (seq_t)518687
};

/* memoisation cache */
static seq_t *cache = NULL;      /* pointer to stored values */
static size_t cache_size = 0;    /* number of values currently stored */

/* compute a(n) for any n >= 1 (n=0 returns 0 as a dummy value) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of offset – harmless */

    /* 0‑based index in the cache */
    size_t idx = (size_t)(n - 1);

    /* extend cache if necessary */
    if (cache_size <= idx) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t*)(cache_size == 0
                                ? malloc(new_size * sizeof(seq_t))
                                : realloc(cache, new_size * sizeof(seq_t)));
        if (!tmp) exit(1);                 /* allocation failure */

        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 12) {
                cache[i] = init[i];        /* initialise the first 12 values */
            } else {
                /* recurrence for i >= 12 (corresponding to n >= 13) */
                cache[i] = 15 * cache[i - 1]
                           - 98 * cache[i - 2]
                           + 366 * cache[i - 3]
                           - 861 * cache[i - 4]
                           + 1323 * cache[i - 5]
                           - 1328 * cache[i - 6]
                           + 840 * cache[i - 7]
                           - 304 * cache[i - 8]
                           + 48  * cache[i - 9];
            }
        }
        cache_size = new_size;
    }

    return cache[idx];
}
