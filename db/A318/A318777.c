/*
 * A318777  Coefficients in expansion of 1/(1 - x - 2*x^5).
 * https://oeis.org/A318777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=a(n-1)+2*a(n-5) for n>=5; a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Expand the cache so it contains values up to n. */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failed */
        cache = tmp;

        /* Compute any new terms. */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i <= 4) {
                cache[i] = (seq_t)1;            /* a(0)…a(4) = 1 */
            } else {
                cache[i] = cache[i - 1] + ((seq_t)2 * cache[i - 5]);
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
