/*
 * A339516  a(n+1) = (a(n) - 2*(n-1)) * (2*n-1), where a(1)=1.
 * https://oeis.org/A339516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n+1) = (a(n) - 2*(n-1))*(2*n-1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for all computed values; cache[0] is unused. */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;
    static idx_t  computed = 0;   /* largest index computed so far */

    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Grow the cache to hold indices up to n. */
    if ((size_t)(n + 1) > cache_sz) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failure */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* Seed the cache on the first call. */
    if (computed == 0) {
        cache[1] = 1;      /* a(1) = 1 */
        computed = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];     /* a(i-1) */
        seq_t i_seq = (seq_t)i;         /* promote index to seq_t */
        seq_t term  = (prev - 2 * (i_seq - 2)) * (2 * i_seq - 3);
        cache[i] = term;
    }
    computed = n;
    return cache[n];
}
