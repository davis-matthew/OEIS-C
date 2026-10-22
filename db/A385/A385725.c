/*
 * A385725  E.g.f. A(x) satisfies A(x) = exp( x*(A(x) + A(i*x) + A(-x) +
 *          A(-i*x))/4 ), where i is the imaginary unit.
 * https://oeis.org/A385725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n‑1)/4⌋} (4k+1) * binomial(n‑1,4k) *
 *            a(4k) * a(n‑1‑4k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t last = (idx_t)-1;  /* last computed index */

    /* allocate or grow cache if needed */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* OOM -> return 0 as a fallback */

        /* initialise new entries to zero */
        memset(new_cache, 0, new_cap * sizeof(seq_t));

        /* copy already computed values if any */
        if (cache) {
            if (last != (idx_t)-1) {
                memcpy(new_cache, cache, (size_t)(last + 1) * sizeof(seq_t));
            }
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            /* iterate over k such that 4*k <= i-1 */
            for (idx_t k = 0; 4 * k <= i - 1; ++k) {
                idx_t idx1 = 4 * k;
                idx_t idx2 = i - 1 - idx1;

                seq_t term = (seq_t)(4 * k + 1);
                term *= binomial((idx_t)(i - 1), idx1);
                term *= cache[idx1];
                term *= cache[idx2];
                sum += term;
            }
            cache[i] = sum;
        }
    }
    last = n;
    return cache[n];
}
