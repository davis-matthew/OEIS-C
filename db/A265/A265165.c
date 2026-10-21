/*
 * A265165  a(n) = sum of the n-th column of the array A265163(n,k). See
 *          Comments for more details.
 * https://oeis.org/A265165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+2) = 2*n*a(n+1) - (n*n - n - 1)*a(n) Initial values: a(0) =
 *            1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* sequence value type and index type are provided by the build system */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array holding computed terms  */
    static idx_t max_computed = 0;/* highest index actually stored */

    /* allocate and initialize if first call */
    if (cache == NULL) {
        /* at least space for n=0 and n=1 */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure: undefined behavior */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        max_computed = 1;
    }

    /* if requested value is already known, return it */
    if (n > max_computed) {
        /* grow the cache to hold up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failure: undefined behavior */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t k = i - 2;  /* n in the recurrence is i-2 */
            seq_t term1 = ((seq_t)(2 * k)) * cache[i - 1];
            seq_t coeff  = (seq_t)((seq_t)k * (seq_t)k - (seq_t)k - (seq_t)1);
            seq_t term2  = coeff * cache[i - 2];
            cache[i] = term1 - term2;
        }
        max_computed = n;
    }

    return cache[n];
}
