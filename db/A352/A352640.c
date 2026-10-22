/*
 * A352640  Expansion of e.g.f. exp(3*sin(x)).
 * https://oeis.org/A352640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 15, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352640 – Expansion of e.g.f. exp(3*sin(x)).
   Formula: a(n) = 3 * Σ_{k=0..⌊(n-1)/2⌋} (-1)^k * binomial(n-1,2*k) * a(n-2*k-1)
   Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=24, a(4)=45,... */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache */
    static size_t computed = 0;/* number of terms already initialised (next to compute) */

    /* Ensure the cache is big enough for index n */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8) new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        idx_t maxk = ((idx_t)i - 1) / 2;
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin = binomial((idx_t)i - 1, 2 * k);
            idx_t j = (idx_t)i - 2 * k - 1;
            seq_t term = bin * cache[j];
            if (k & 1) term = -term;   /* multiply by (-1)^k */
            sum += term;
        }
        cache[i] = sum * (seq_t)3;
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
