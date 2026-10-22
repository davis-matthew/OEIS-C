/*
 * A338852  a(n) = (7*floor(a(n-1)/3)) + (a(n-1) mod 3) with a(1) = 3.
 * https://oeis.org/A338852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*floor(a(n-1)/3) + (a(n-1) mod 3), a(1) = 3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    if (n == 0) return 0; /* offset is 1; 0 is unused */

    if (cached_size < n) {
        /* Extend cache to hold up to n elements */
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 (undefined) */
            return 0;
        }
        cache = tmp;
        /* Compute missing values */
        for (idx_t i = cached_size; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)3;          /* a(1) */
            } else {
                seq_t prev = cache[i - 1];
                seq_t div  = prev / (seq_t)3;
                seq_t rem  = prev % (seq_t)3;
                cache[i] = ((seq_t)7 * div) + rem;
            }
        }
        cached_size = n;
    }

    return cache[n - 1];
}
