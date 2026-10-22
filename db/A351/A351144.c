/*
 * A351144  G.f. A(x) satisfies: A(x) = 1 + x^2 * A(x/(1 - 3*x)) / (1 - 3*x).
 * https://oeis.org/A351144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351144
   a(0) = 1, a(1) = 0
   a(n) = Sum_{k=0}^{n-2} binomial(n-2,k) * 3^k * a(n-k-2)
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of already computed values.
       cache[i] contains a(i).  `cap` is the size of the allocated cache. */
    static seq_t *cache = (seq_t *)0;
    static size_t cap   = 0;

    /* Grow cache if necessary.  We want indices 0..n inclusive. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache;
        if (cache == (seq_t *)0) {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (new_cache == (seq_t *)0) {
            /* Allocation failed – terminate the program.  In the harness
               this will never happen for the expected input sizes. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Compute the new entries. */
        for (idx_t i = cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 0;
            } else {
                idx_t N = i - 2;               /* n-2 in the recurrence */
                seq_t sum  = 0;                /* running sum of terms */
                seq_t binom = 1;               /* C(N,0) */
                seq_t pow3  = 1;               /* 3^0 */
                for (idx_t k = 0; k <= N; ++k) {
                    seq_t term = binom * pow3 * cache[(size_t)(i - k - 2)];
                    sum += term;
                    if (k < N) {
                        /* binom = binom * (N - k) / (k + 1) */
                        binom = binom * (N - k) / (k + 1);
                        pow3  *= 3;
                    }
                }
                cache[i] = sum;
            }
        }
        cap = new_cap;
    }
    return cache[(size_t)n];
}
