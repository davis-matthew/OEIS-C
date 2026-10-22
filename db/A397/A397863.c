/*
 * A397863  G.f. A(x) satisfies A(x) = 1 + x * (1+x)^2 * A(x * (1+x)^3).
 * https://oeis.org/A397863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) + Σ_{k=0}^{n-1} binomial(3*k+2, n-k-1) * a(k) with
 *            a(0) = 1. This function computes a(n) for any n ≥ 0, memoising
 *            results in a static cache so that calls in any order are
 *            efficient
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i < cap */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* current cache capacity */

    idx_t start = cap;         /* the first index that needs computing */

    /* Expand cache if necessary */
    if (n >= cap) {
        idx_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;   /* capacity needed for n */
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure → 0 */
        if (cache) {
            memcpy(tmp, cache, old_cap * sizeof(seq_t));
            free(cache);
        }
        /* initialise newly available entries to 0 */
        for (idx_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
        start = old_cap;
    }

    /* Compute all values from 'start' up to 'n' */
    for (idx_t m = start; m <= n; ++m) {
        if (m == 0) {             /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t num = 3*k + 2;
            idx_t den = m - k - 1;
            seq_t bin = binomial(num, den);
            sum += bin * cache[k];   /* all arithmetic in seq_t */
        }
        cache[m] = sum;
    }

    return cache[n];
}
