/*
 * A292953  E.g.f.: exp(-1/2! * x^2 * exp(x)).
 * https://oeis.org/A292953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) from EGF exp(-x^2/2 * exp(x))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 24, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A292953 */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static idx_t  computed  = 0; /* largest n for which cache[i] is valid */

    /* enlarge storage if necessary */
    if (n >= (idx_t)cache_len) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache_len = (size_t)(n + 1);
        /* initialise the first value */
        if (cache_len > 0) {
            cache[0] = (seq_t)1;
            computed = 0;
        }
    }

    /* compute missing terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            idx_t np = i - 1;          /* n in the recurrence */
            seq_t sum = (seq_t)0;

            seq_t bin = (seq_t)1;      /* binom(np,0) */
            for (idx_t d = 1; d <= np; ++d) {
                bin = (bin * (seq_t)(np - d + 1)) / (seq_t)d;
                seq_t ak   = cache[np - d];
                seq_t part = bin * ak * (seq_t)d * (seq_t)(d + 1);
                sum += part;
            }
            seq_t tmp = sum / (seq_t)2;
            cache[i] = -tmp;
        }
        computed = n;
    }

    return cache[n];
}
