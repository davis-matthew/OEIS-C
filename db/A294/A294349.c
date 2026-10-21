/*
 * A294349  Product of first n terms of the binomial transform of the Lucas
 *          numbers (A000032).
 * https://oeis.org/A294349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n} b_k, with * b_0 = 2, b_1 = 3, and b_k =
 *            3·b_{k-1} – b_{k-2} for k ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of computed a(n) values            */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;
    static idx_t last    = (idx_t)-1;   /* last index stored in cache */

    /* last two values of the binomial‑transform sequence */
    static seq_t b_prev1; /* b_{last}   */
    static seq_t b_prev2; /* b_{last-1} */

    /* initialise on first call */
    if (cache == NULL) {
        cap = 16;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)2;          /* a(0) = 2 */
        last    = 0;
        b_prev1 = (seq_t)2;           /* b_0 = 2 */
        b_prev2 = (seq_t)0;           /* unused until last ≥ 1 */
    }

    /* return cached value if already computed */
    if (n <= last)
        return cache[(size_t)n];

    /* extend cache up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t b_i;
        if (i == 1)                /* first special case */
            b_i = (seq_t)3;       /* b_1 = 3 */
        else
            b_i = 3 * b_prev1 - b_prev2;   /* recurrence */

        /* grow cache if required */
        if (last + 1 >= cap) {
            size_t new_cap = cap * 2;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* compute new a(i) = a(i-1) * b_i */
        cache[(size_t)(last + 1)] = cache[(size_t)last] * b_i;

        /* shift b values for next iteration */
        b_prev2 = b_prev1;
        b_prev1 = b_i;
        ++last;
    }

    return cache[(size_t)n];
}
