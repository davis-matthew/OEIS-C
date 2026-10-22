/*
 * A368718  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * k^5 / k!.
 * https://oeis.org/A368718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -n*a(n-1) + n^5, with a(0) = 0
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static idx_t cache_len = 0;      /* number of entries currently stored   */
    static idx_t cache_cap = 0;      /* capacity of allocated cache array */

    /* Grow cache if necessary to reach index n. */
    if (n >= cache_len) {
        /* Determine new capacity (next power of two) */
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;          /* a(0) = 0 */
            } else {
                /* compute i^5 in seq_t */
                seq_t i_seq = (seq_t)i;
                seq_t i5 = i_seq;
                i5 = i5 * i_seq; /* i^2 */
                i5 = i5 * i_seq; /* i^3 */
                i5 = i5 * i_seq; /* i^4 */
                i5 = i5 * i_seq; /* i^5 */

                /* a(i) = -i * a(i-1) + i^5 */
                cache[i] = -((seq_t)i * cache[i - 1]) + i5;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
