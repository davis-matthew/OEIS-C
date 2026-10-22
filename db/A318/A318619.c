/*
 * A318619  a(0) = 0, a(1) = 1; for n > 1, a(n) = Sum_{k=0..n-2} a(k) XOR
 *          a(n-k-2).
 * https://oeis.org/A318619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1; for n>1
 * a(n) = Σ_{k=0}^{n-2} [ a(k) XOR a(n-k-2) ] */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache and tracking of last computed index */
    static seq_t *cache       = NULL;
    static size_t cache_size  = 0;
    static idx_t last_computed = 1;   /* we know a(0) and a(1) at start */

    /* initial allocation for the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;
        cache[1] = 1;
    }

    /* grow cache if needed and compute new terms only once */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;
    }

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* Compute the XOR convolution for term i */
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum += cache[k] ^ cache[i - 2 - k];
            }
            cache[i] = sum;
        }
        last_computed = n;
    }

    return cache[n];
}
