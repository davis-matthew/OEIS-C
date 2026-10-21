/*
 * A260884  Number of set partitions of a 2n-set into even blocks which have
 *          even length minus the number of partitions into even blocks which
 *          have odd length.
 * https://oeis.org/A260884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(2n) in exp(1 - cosh(x)) times (2n)!.
 *            Recurrence: a(0) = 1; for n >= 1: a(n) = - sum_{j=1..n} a(n-j) *
 *            binom(2n-1, 2j-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use only integer arithmetic and small helpers.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache[i] holds a(i) */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t  max_n_computed = 0; /* largest n for which cache is defined */

    /* first call: initialise cache with a(0) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        max_n_computed = 0;
    }

    /* extend cache up to n if necessary */
    if (n > max_n_computed) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute values from max_n_computed+1 up to n */
        for (idx_t i = max_n_computed + 1; i <= n; ++i) {
            /* Ni = 2*i - 1, fits in int for i <= 10000 */
            int Ni = (int)(2 * (int)i - 1);

            seq_t sum = 0;   /* sum of a(i-j) * binom ... */
            seq_t coef = 1;  /* current binomial coefficient C(Ni, k) */
            int j = 1;

            for (int k = 1; k <= Ni; ++k) {
                /* update coef to C(Ni, k) */
                coef = coef * (seq_t)(Ni - (k - 1)) / (seq_t)k;

                if (k & 1) { /* k = 2*j - 1 */
                    sum -= cache[i - (idx_t)j] * coef;
                    ++j;
                }
            }
            cache[i] = sum;   /* a(i) = -sum */
        }
        max_n_computed = n;
    }

    return cache[n];
}
