/*
 * A309790  G.f. A(x) satisfies: A(x) = 2*x*(1 - x)*A(x^2) + x/(1 - x).
 * https://oeis.org/A309790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, hear
 * Formula:   a(0) = 0; a(2*n+2) = -2*a(n) + 1, a(2*n+1) = 2*a(n) + 1 for n >=
 *            0 (derived from the generating‑function recurrence)
 * Range:     i8 n <= 254, i16 n <= 9677, i32 n <= 9677, i64 n <= 9677,
 *            i128 n <= 9677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;            /* allocated size of cache */
    static idx_t last = 0;            /* largest index currently computed */

    /* allocate (or reallocate) cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n + 1U);
        cap = (size_t)n + 1U;
        cache[0] = 0;                 /* a(0) = 0 */
        last = 0;
    } else if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;    /* unlikely, but keep type correct */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
        } else if ((i & 1U) == 0U) {        /* even index: a(2*n+2) */
            idx_t k = (i - 2U) / 2U;        /* n = (i-2)/2 */
            cache[i] = -((seq_t)2 * cache[k]) + 1;
        } else {                           /* odd index: a(2*n+1) */
            idx_t k = (i - 1U) / 2U;        /* n = (i-1)/2 */
            cache[i] = ((seq_t)2 * cache[k]) + 1;
        }
    }
    last = n;
    return cache[n];
}
