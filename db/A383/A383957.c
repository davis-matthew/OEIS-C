/*
 * A383957  Sum of the legs of the unique primitive Pythagorean triple whose
 *          inradius is A000108(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A383957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Catalan(n))^2 + 4*Catalan(n) + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"    /* provides seq_t catalan(idx_t k) */

/* Compute a(n) for OEIS A383957 */
seq_t a(idx_t n)
{
    /* Cache Catalan numbers up to the maximum requested value   */
    static seq_t *cat_cache = NULL;
    static idx_t cat_max   = (idx_t)-1;

    /* Grow cache if necessary */
    if (cat_cache == NULL || n > cat_max) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cat_cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();       /* unlikely, but safe */
        cat_cache = new_cache;

        /* Initialize at start if this is the first allocation */
        if (cat_max == (idx_t)-1) {
            cat_cache[0] = (seq_t)1;   /* Catalan(0) = 1 */
            cat_max = 0;
        }

        /* Compute Catalan numbers from cat_max+1 up to n */
        for (idx_t i = cat_max + 1; i <= n; ++i) {
            /* Recurrence: C(i) = C(i-1) * (4*(i-1)+2) / (i+1) */
            idx_t k = i - 1;
            seq_t numerator   = cat_cache[k] * (((seq_t)4 * k) + 2);
            seq_t denominator = (seq_t)i + 1;
            cat_cache[i] = numerator / denominator;
        }
        cat_max = n;
    }

    /* Use Catalan(n) from the cache to compute a(n) */
    seq_t C = cat_cache[n];
    seq_t ret = ((seq_t)2 * C * C) + ((seq_t)4 * C) + 1;
    return ret;
}
