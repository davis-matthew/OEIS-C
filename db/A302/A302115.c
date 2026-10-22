/*
 * A302115  a(n) = 16*(n-1)*a(n-1) + ((-1)^n)*(4/3)*Product_{k=0..n-1} (2*k-3)
 *          with a(0) = 0.
 * https://oeis.org/A302115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A302115:
   a(0) = 0
   a(n) = 16*(n-1)*a(n-1) + ((-1)^n)*(4/3)*∏_{k=0}^{n-1}(2*k-3)   for n ≥ 1
   The product is taken over all k from 0 to n‑1. */

#include <stdlib.h>   /* realloc, abort */

seq_t a(idx_t n)
{
    /* Cache for already computed terms and for the absolute product ∏|2k-3|. */
    static seq_t *a_cache = NULL;   /* a(n) values                 */
    static seq_t *p_cache = NULL;   /* product of absolute values   */
    static size_t cache_size = 0;   /* number of cached entries     */

    size_t idx = (size_t)n;   /* conversion for array indexing */

    /* Grow cache to include up to index n */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;                    /* indices 0 .. idx */

        /* Allocate new arrays (or grow the old ones) */
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        p_cache = (seq_t *)realloc(p_cache, new_size * sizeof(seq_t));
        if (!a_cache || !p_cache)
            abort();   /* allocation failed */

        /* First call: initialise a(0) = 0 and empty product = 1 */
        if (cache_size == 0) {
            a_cache[0] = 0;
            p_cache[0] = 1;
            cache_size = 1;
        }

        /* Compute values from the first missing index up to idx */
        for (size_t i = cache_size; i <= idx; ++i) {
            /* Compute |2*(i-1)-3| */
            seq_t factor;
            if (i == 1)
                factor = (seq_t)3;
            else if (i == 2)
                factor = (seq_t)1;
            else
                factor = (seq_t)(2*i - 5);   /* i ≥ 3 ⇒ 2*i‑5 ≥ 1 */

            /* Update the absolute product */
            p_cache[i] = p_cache[i-1] * factor;

            /* (4/3) * product; division is exact */
            seq_t term = (seq_t)4 * p_cache[i];
            term /= (seq_t)3;

            /* Sign of the additive term:
               + for i = 1 or even i ≥ 2,
               - for odd i ≥ 3                        */
            int sign_is_positive = 1;   /* default + */
            if (i == 1) {
                sign_is_positive = 1;
            } else if (i & 1) {        /* odd i */
                sign_is_positive = 0;   /* negative for i ≥ 3 */
            } else {
                sign_is_positive = 1;   /* even i ≥ 2 */
            }

            /* a(i) = 16*(i-1)*a(i-1) + sign*term */
            seq_t mult = (seq_t)16 * (seq_t)(i-1);
            mult = mult * a_cache[i-1];

            if (sign_is_positive)
                a_cache[i] = mult + term;
            else
                a_cache[i] = mult - term;
        }

        cache_size = new_size;
    }

    return a_cache[idx];
}
