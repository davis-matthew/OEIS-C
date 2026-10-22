/*
 * A318783  Expansion of Product_{k>=1} 1/(1 - x^k)^(d(k)-1), where d(k) =
 *          number of divisors of k (A000005).
 * https://oeis.org/A318783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* product_{k>=1} 1/(1 - x^k)^(d(k)-1)   */
/* where d(k) is the number of divisors of k. */
/* Computed by dynamic programming over all k <= n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache is re‑computed when asked for a larger index        */
    static seq_t *cache = NULL;   /* array of computed a(i) values       */
    static idx_t cached_n = 0;    /* highest n for which cache is valid */

    if (!cache || n > cached_n) {
        /* free old cache if any */
        free(cache);

        /* allocate new cache, zeroed */
        cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on OOM */

        /* base case a(0) = 1 */
        cache[0] = 1;

        /* divisor counts d[1..n] */
        idx_t *d = (idx_t*)calloc((size_t)(n + 1), sizeof(idx_t));
        if (!d) {
            free(cache); cache = NULL; return (seq_t)0;
        }
        /* simple O(n log n) sieve for divisor counts */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                ++d[j];
            }
        }

        /* dynamic programming over k from 1 to n          */
        /* each factor (1 - x^k)^-(d(k)-1) is expanded,      */
        /* using the standard unbounded knapsack iteration. */
        for (idx_t k = 1; k <= n; ++k) {
            idx_t e = d[k] - 1;      /* exponent for part size k   */
            for (idx_t rep = 0; rep < e; ++rep) {   /* repeat e times          */
                for (idx_t j = k; j <= n; ++j) {
                    cache[j] += cache[j - k];
                }
            }
        }

        free(d);
        cached_n = n;
    }
    return cache[n];
}
