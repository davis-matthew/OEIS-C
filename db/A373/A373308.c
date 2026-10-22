/*
 * A373308  Expansion of Product_{n>=0} (1 - x^(2^n))^3.
 * https://oeis.org/A373308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of A(x) where A(x) = Product_{m>=0}(1 -
 *            x^(2^m))^3 Recurrence: a(0)=1; for n>=0 a(n) = Σ_{k=0..3} p[k] *
 *            a((n-k)/2) if (n-k) is even where p = {1, -3, 3, -1}
 * Range:     i8 n <= 30, i16 n <= 1022, i32 n <= 8590, i64 n <= 8590,
 *            i128 n <= 8590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;          /* number of initialized elements */

    /* cast for C++ */
    size_t idx = (size_t)n;

    /* Ensure cache is large enough */
    if (idx + 1 > cached_len) {
        if (cached_len == 0) {
            size_t init_cap = 1;
            cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
            cache[0] = (seq_t)1;        /* a(0) = 1 */
            cached_len = 1;
        }
        if (cached_len < idx + 1) {
            size_t new_cap = cached_len * 2;
            if (new_cap < idx + 1)
                new_cap = idx + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
        }

        /* Coefficients of (1 - 3x + 3x^2 - x^3) */
        const int coeff[4] = {1, -3, 3, -1};

        /* Compute missing values up to idx */
        for (size_t i = cached_len; i <= idx; ++i) {
            seq_t val = 0;
            for (int k = 0; k < 4; ++k) {
                if (i < (size_t)k) continue;
                size_t diff = i - k;
                if ((diff & 1) == 0) {          /* (i-k) even */
                    size_t half = diff >> 1;
                    val += cache[half] * (seq_t)coeff[k];
                }
            }
            cache[i] = val;
        }
        cached_len = idx + 1;
    }
    return cache[idx];
}
