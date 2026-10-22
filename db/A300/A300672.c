/*
 * A300672  Expansion of 1/(1 - Sum_{p prime, k>=1} x^(p^k)/(1 - x^(p^k))).
 * https://oeis.org/A300672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = Σ_{k=2}^{n} bigomega(k) · a(n-k)
 *            (inverse transform of A001222, where bigomega(k) is the total
 *            number of prime factors of k counted with multiplicity)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 84, u128 n <= 165,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* cache for previously computed values */
    static seq_t *cache = NULL;       /* a[0] … a[cap-1] */
    static idx_t  cap   = 0;          /* current cache capacity (size) */

    /* grow the cache so that it contains at least a[0 … n] */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* memory failure */
        cache = tmp;
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0; /* initialise */
        cap = new_cap;
    }

    /* initialise the base terms */
    if (cap > 0 && cache[0] == 0) cache[0] = 1;
    if (cap > 1 && cache[1] == 0) cache[1] = 0;

    /* compute any missing values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;     /* already computed */
        seq_t sum = 0;
        for (idx_t k = 2; k <= i; ++k) {
            sum += cache[i - k] * bigomega(k);
        }
        cache[i] = sum;
    }

    return cache[n];
}
