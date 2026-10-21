/*
 * A294640  G.f. A(x) = Sum_{n>=0} x^n/a(n) satisfies: A(x) = A(x^2) +
 *          Integral A(x^2) dx.
 * https://oeis.org/A294640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1; For n > 0: if n is even then a(n) = a(n/2); if n is
 *            odd then a(n) = n * a((n-1)/2)
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 254, u64 n <= 2046,
 *            u128 n <= 4100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;      /* array holding a(0) .. a(N) */
    static idx_t  cache_size = 0;    /* number of elements allocated */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – terminate (no proper error handling needed) */
            abort();
        }
        cache = new_cache;

        /* initialize new entries */
        if (cache_size == 0) {               /* first use → set a(0)=1 */
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        for (idx_t i = cache_size; i < new_size; ++i) {
            if ((i & 1) == 0) {                     /* even */
                cache[i] = cache[i >> 1];
            } else {                                /* odd */
                cache[i] = cache[(i - 1) >> 1] * (seq_t)i;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
