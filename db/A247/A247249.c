/*
 * A247249  a(n) = (2*n-1)*a(n-1) + (n-1)*a(n-2) with a(0) = a(1) = 1.
 * https://oeis.org/A247249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) + (n-1)*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static idx_t len = 0;   /* number of initialized terms */
    static idx_t cap = 0;   /* allocated capacity */

    /* initialise cache with the first two terms */
    if (!initialized) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
        cap = 2;
        initialized = true;
    }

    /* return immediately if the term is already cached */
    if (n < len) {
        return cache[n];
    }

    /* increase capacity if necessary */
    idx_t required = n + 1;
    if (required > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        cache = tmp;
        cap = required;
    }

    /* compute missing terms iteratively */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term1 = ((seq_t)(2 * i - 1)) * cache[i - 1];
        seq_t term2 = ((seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term1 + term2;
    }
    len = required;

    return cache[n];
}
