/*
 * A397449  a(n) = Sum_{k=0..n-1} a(k)*(-2)^k, with a(0) = 1.
 * https://oeis.org/A397449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(n) = ∑_{k=0}^{n-1} a(k)*(-2)^k for n ≥ 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms and the next power of -2 */
    static seq_t *cache = NULL;   /* array of a(i) */
    static size_t cap   = 0;      /* allocated size */
    static size_t sz    = 0;      /* number of computed terms (cache[0..sz-1]) */
    static seq_t pow_cur = 0;     /* (-2)^{sz-1}  : the power used for the next term */

    /* Initialise on first call: a(0)=1, a(1)=1, pow_cur = (-2)^1 = -2 */
    if (cache == NULL) {
        cap = 2;                          /* enough for indices 0 and 1 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        sz = 2;
        pow_cur = (seq_t)(-2);
    }

    /* Return cached value if already computed */
    if ((size_t)n < sz)
        return cache[n];

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            return (seq_t)0;           /* reallocation failed */
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Extend the sequence from sz up to n inclusive */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        /* For i = 2 the factor is (-2)^{1} + 1 = -1, etc. */
        seq_t term = pow_cur + (seq_t)1;        /* (-2)^{i-1} + 1 */
        cache[i] = cache[i - 1] * term;        /* a(i) = a(i-1) * term */
        pow_cur = pow_cur * (seq_t)(-2);        /* advance to (-2)^{i} for next step */
    }

    sz = (size_t)n + 1;
    return cache[n];
}
