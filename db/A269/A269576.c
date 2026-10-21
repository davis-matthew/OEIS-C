/*
 * A269576  a(n) = Product_{i=1..n} (4^i - 3^i).
 * https://oeis.org/A269576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} (4^i - 3^i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product form; no special cases are required. */

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* static cache: 1-indexed array of computed values */
    static seq_t *cache   = NULL;          /* array pointer */
    static idx_t  cachedN = 0;             /* current maximum index stored */

    if (n == 0)   /* offset is 1, but protect against 0 */
        return (seq_t)0;

    /* grow cache if necessary */
    if (n > cachedN) {
        if (!cache) {
            cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        }
    }

    /* compute missing terms up to n */
    if (cachedN < n) {
        /* seed product of zero terms to make the recurrence simpler */
        if (cachedN == 0) {
            cache[0] = (seq_t)1;   /* product for i = 0 (unused) */
        }
        for (idx_t i = cachedN + 1; i <= n; ++i) {
            seq_t pow4 = seq_pow((seq_t)4, i);
            seq_t pow3 = seq_pow((seq_t)3, i);
            seq_t term = pow4 - pow3;          /* 4^i - 3^i, i ≥ 1 */
            if (i == 1)
                cache[i] = term;               /* a(1) = 1 */
            else
                cache[i] = cache[i - 1] * term;/* a(i) = a(i-1) * term */
        }
        cachedN = n;
    }

    return cache[n];
}
