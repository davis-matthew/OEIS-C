/*
 * A354419  Expansion of e.g.f. log(1+4*x) * exp(x)/4.
 * https://oeis.org/A354419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1; a(n) = (-4*n + 5)*a(n-1) + 4*(n-1)*a(n-2) +
 *            1 (n ≥ 2) Recurrence derived from the e.g.f. log(1+4*x)*exp(x)/4
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure stdlib is available for size_t, realloc, NULL */
#include <stdlib.h>

/* external types provided by the build environment */
extern seq_t a(idx_t n);

/* Implementation of the sequence function */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static memoisation storage */
    static seq_t *cache = NULL;   /* array of computed a(k) values  */
    static size_t cap   = 0;      /* allocated capacity (number of elements) */
    static size_t len   = 0;      /* next index to compute (len == number of initialized elements) */

    /* make sure the cache has room for indices 0 .. n */
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        size_t new_cap = (cap == 0) ? 2 : cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure, unlikely in tests */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any values that are still missing, up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t coeff1 = (seq_t)-4 * (seq_t)i + (seq_t)5;      /* (-4*i + 5) */
            seq_t term1  = coeff1 * cache[i - 1];                /* coeff1 * a(i-1) */
            seq_t term2  = (seq_t)4 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 + term2 + (seq_t)1;
        }
    }

    len = (size_t)(n + 1);
    return cache[n];
}
