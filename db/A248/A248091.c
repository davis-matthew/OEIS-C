/*
 * A248091  Expansion of
 *          x^3*(1-2x-x^2-x^3+x^4+x^5)/((1+x)*(1-3x+x^2-x^3+3x^4)).
 * https://oeis.org/A248091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Coefficients of generating function x^3*(1-2x-x^2-x^3+x^4+x^5)
 *            /* /((1+x)*(1-3x+x^2-x^3+3x^4))
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 52, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Constant terms of the numerator for n = 0..8 */
static const int N_vals[9] = {0, 0, 0, 1, -2, -1, -1, 1, 1};

seq_t a(idx_t n)
{
    if ((idx_t)cache_size <= n) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp && newsize > 0)
            abort();                     /* memory allocation failed */
        cache = tmp;
        size_t oldsize = cache_size;
        cache_size = newsize;
        for (size_t i = oldsize; i < newsize; ++i) {
            seq_t a_nm1 = (i >= 1) ? cache[i - 1] : (seq_t)0;
            seq_t a_nm2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
            seq_t a_nm4 = (i >= 4) ? cache[i - 4] : (seq_t)0;
            seq_t a_nm5 = (i >= 5) ? cache[i - 5] : (seq_t)0;

            /* Part from the left‑hand side of the recurrence */
            seq_t A = a_nm1 * 2 + a_nm2 * 2;
            int Nc = 0;
            if (i < 9) Nc = N_vals[i];
            if (Nc > 0) {
                A += (seq_t)Nc;
            } else if (Nc < 0) {
                A -= (seq_t)(-Nc);
            }

            /* Subtracted part (right‑hand side of the recurrence) */
            seq_t B = a_nm4 * 2 + a_nm5 * 3;

            cache[i] = A - B;               /* guaranteed nonnegative */
        }
    }
    return cache[(size_t)n];
}
