/*
 * A296590  a(n) = Product_{k=0..n} binomial(2*n - k, k).
 * https://oeis.org/A296590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0..n} binomial(2*n - k, k) * = a(n-1) *
 *            binomial(2*n - 1, n) with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated size */
    static size_t filled = 0;    /* number of terms computed (max index + 1) */

    /* Initialise for the first call (or when cache is empty) */
    if (filled == 0) {
        cap = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        filled = 1;
    }

    /* Resize cache if needed to hold up to index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = filled; i <= (size_t)n; ++i) {
        idx_t idx      = (idx_t)i;
        idx_t top      = (idx_t)(2 * i - 1);   /* 2*i - 1 fits in idx_t */
        seq_t mult     = binomial(top, idx);
        cache[i] = cache[i - 1] * mult;
    }
    filled = (size_t)n + 1;

    return cache[(size_t)n];
}
