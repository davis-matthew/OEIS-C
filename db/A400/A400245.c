/*
 * A400245  G.f. A(x) satisfies A(x) = A(x^2/(1 - x)^3) / (1 - x)^3, with A(0)
 *          = 1.
 * https://oeis.org/A400245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = sum_{k=0..floor(n/2)} binomial(n+k+2, 3*k+2) *
 *            a(k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* g.f. table for a[0 .. len-1] */
    static size_t cap  = 0;       /* allocated capacity of cache        */
    static size_t len  = 0;       /* number of actually computed terms  */

    /* first call – create cache and init a[0] */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t*)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        len = 1;
    }

    /* ensure cache big enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* optional: zero new slots */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t lim = i >> 1;               /* floor(i/2) */
        for (idx_t k = 0; k <= lim; ++k) {
            idx_t a0 = i + k + 2;
            idx_t b0 = 3 * k + 2;
            seq_t term = binom(a0, b0);
            term = term * cache[k];
            sum += term;
        }
        cache[i] = sum;
    }

    len = (size_t)n + 1;
    return cache[n];
}
