/*
 * A368787  a(n) = (n+1) * (n!)^2 * Sum_{k=1..n} 1/((k+1) * (k!)^2).
 * https://oeis.org/A368787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n) = (n+1)*n*a(n-1)+1 for n>=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static idx_t cache_size = 0;  /* number of elements currently stored (>=0) */

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;                    /* need indices 0..n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        if (old_size == 0) {                       /* initialise first entry */
            cache[0] = 0;
            old_size = 1;
        }

        for (idx_t i = old_size; i <= n; ++i) {    /* compute missing values */
            seq_t prev = cache[i - 1];
            cache[i] = ((seq_t)(i + 1) * (seq_t)i * prev) + 1;
        }
        cache_size = new_size;
    }
    return cache[n];
}
