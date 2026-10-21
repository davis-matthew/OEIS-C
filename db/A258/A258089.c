/*
 * A258089  a(n) = n for n = 0..3; for n>3, a(n) = 4*a(n-1) + 3*a(n-2) +
 *          2*a(n-3) + a(n-4).
 * https://oeis.org/A258089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) + 2*a(n-3) + a(n-4), a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

seq_t a(idx_t n)
{
    /* Expand the cache if the request exceeds the current size */
    if (a_cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp)                          /* allocation failed */
            return (seq_t)0;
        a_cache = tmp;

        /* Compute missing terms */
        for (size_t i = a_cache_len; i < new_len; ++i) {
            if (i < 4) {
                a_cache[i] = (seq_t)i;
            } else {
                a_cache[i] = (seq_t)(4 * a_cache[i-1] +
                                    3 * a_cache[i-2] +
                                    2 * a_cache[i-3] +
                                    a_cache[i-4]);
            }
        }
        a_cache_len = new_len;
    }
    return a_cache[(size_t)n];
}
