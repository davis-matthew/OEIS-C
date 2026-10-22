/*
 * A352638  Expansion of e.g.f. 1/(1 - 3*sin(x)).
 * https://oeis.org/A352638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3 * Σ_{k=0..⌊(n-1)/2⌋} (-1)^k *
 *            binomial(n,2k+1) * a(n-2k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                 /* malloc, realloc, exit */
#include "../utils/binomial.h"      /* binomial(n,k) -> seq_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* store computed values */
    static size_t cap   = 0;         /* current capacity of cache */

    /* Initialise cache with a(0) = 1 */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cap     = 1;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Return already computed value */
    if (cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    /* Compute a(n) using the recurrence */
    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;
    idx_t max_k = (n - 1) / 2;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = 2 * k + 1;                     /* odd index */
        seq_t bin = binomial(n, m);              /* binomial coefficient */
        seq_t term = bin * cache[(size_t)(n - m)];/* term = bin * a(n-m) */
        if ((k & 1) == 0)
            sum_pos += term;                     /* even k → + */
        else
            sum_neg += term;                     /* odd  k → - */
    }

    seq_t result = (sum_pos - sum_neg) * (seq_t)3;
    cache[(size_t)n] = result;
    return result;
}
