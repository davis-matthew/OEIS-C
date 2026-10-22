/*
 * A330797  Evaluation of the Stirling cycle polynomials at -1/2 and
 *          normalized with (-2)^n.
 * https://oeis.org/A330797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (3-2*n)*a(n-1) with a(0)=1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* allocate space for new entries  */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */

        /* copy previously cached values */
        if (cache)
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));

        /* compute memo values from cache_size up to n */
        for (idx_t i = (cache_size == 0 ? 0 : cache_size); i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = (seq_t)1;          /* a(0) = 1 */
            } else {
                seq_t coeff = (seq_t)3 - ((seq_t)i << 1);  /* coeff = 3-2*i */
                new_cache[i] = coeff * new_cache[i-1];
            }
        }

        /* free old cache and install new one */
        if (cache) free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
