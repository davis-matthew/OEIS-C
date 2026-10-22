/*
 * A332258  Expansion of e.g.f. 1 / (1 + x - sinh(x)).
 * https://oeis.org/A332258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=2}^{⌈n/2⌉} binomial(n,2k-1)*a(n−2k+1)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* memoization cache: store already computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t size  = 0;   /* number of values currently computed (>=1 if a[0] is set) */

    /* ensure capacity for index n */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? (size_t)n + 1 : cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* initialise a[0] if necessary */
    if (size == 0) {
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* compute missing terms up to n */
    for (; size <= (size_t)n; ++size) {
        idx_t idx = (idx_t)size;
        if (idx == 1 || idx == 2) {
            cache[idx] = (seq_t)0;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t limit = (idx + 1) / 2;   /* ceil(idx/2) */
        for (idx_t k = 2; k <= limit; ++k) {
            idx_t binom_idx = 2 * k - 1;
            seq_t term = binomial(idx, binom_idx) * cache[idx - 2 * k + 1];
            sum += term;
        }
        cache[idx] = sum;
    }
    return cache[n];
}
