/*
 * A336614  Number of n X n (0,1)-matrices A over the reals such that A^2 is
 *          the transpose of A.
 * https://oeis.org/A336614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + (n^2 - 3*n + 2)*a(n-3) with initial values
 *            a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* memory failure (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
    }

    /* return known values directly */
    if (n < cache_size) return cache[n];

    /* grow cache if necessary */
    idx_t old_size = cache_size;
    if (n + 1 > old_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* memory failure (unlikely) */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t i_seq = (seq_t)i;
        seq_t coeff = (i_seq * i_seq - (seq_t)3 * i_seq + (seq_t)2);
        seq_t val = (seq_t)2 * a1 + coeff * a3;
        cache[i] = val;
    }

    return cache[n];
}
