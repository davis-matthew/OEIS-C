/*
 * A395204  a(n) = (2*n)! * [x^(2*n)] (f(x)^2 + f(-x)^2)/2 and f(x) = 1/sqrt(1
 *          + 2*sqrt(2)*x + x^2).
 * https://oeis.org/A395204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * [x^(2n)] (1+x^2)/(1-6x^2+x^4) Recurrence: a(0)=1,
 *            a(1)=14, a(n) = (2n)*(2n-1) * ( 6*a(n-1) - (2n-2)*(2n-3)*a(n-2)
 *            ) for n>=2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of cached elements (>=2 after init) */

    /* initialise cache on first call */
    if (size == 0) {
        size_t init_size = n + 1;
        if (init_size < 2) init_size = 2;          /* need at least a(0), a(1) */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        if (!cache) return (seq_t)0;              /* out‑of‑memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        size = init_size;
    }
    /* grow cache if required */
    else if (size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*new_size);
        if (!tmp) return (seq_t)0;              /* out‑of‑memory */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = size; i <= n; ++i) {
            /* small integers used in recurrence */
            idx_t two_i   = (idx_t)2 * i;
            idx_t two_1   = two_i - 1;
            idx_t two_2   = two_i - 2;
            idx_t two_3   = two_i - 3;

            seq_t term1 = (seq_t)6 * cache[i-1];
            seq_t term2 = (seq_t)two_2 * (seq_t)two_3 * cache[i-2];
            seq_t temp  = term1 - term2;

            cache[i] = (seq_t)two_i * (seq_t)two_1 * temp;
        }
        size = new_size;
    }

    return cache[n];
}
