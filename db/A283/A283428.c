/*
 * A283428  Starting with a(1)=3, a(2)=4, a(n)=sum of digits of a(n-1) + sum
 *          of digits of a(n-2).
 * https://oeis.org/A283428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(a(n-1)) + digit_sum(a(n-2)); Initial values:
 *            a(1)=3, a(2)=4
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;         /* dynamic array of computed terms */
static size_t   cached = 0;          /* number of terms already computed */

seq_t a(idx_t n)
{
    /* OEIS offset: n >= 1 */
    if (n == 0) return 0;

    /* Expand the cache if necessary */
    if ((idx_t)cached < n) {
        size_t new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();   /* out of memory */

        /* Compute missing terms */
        for (idx_t i = cached; i < n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)3;
            } else if (i == 1) {
                cache[1] = (seq_t)4;
            } else {
                cache[i] = digit_sum(cache[i - 1]) + digit_sum(cache[i - 2]);
            }
        }
        cached = n;
    }

    return cache[(size_t)(n - 1)];
}
