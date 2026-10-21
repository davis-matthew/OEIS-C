/*
 * A289922  Coefficients of 1/([1+r] - [1+2r]x + [1+3r]x^2 - ...), where [ ] =
 *          floor and r = 19/21.
 * https://oeis.org/A289922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) is the coefficient of the power series 1 / ( [1+r] -
 *            [1+2r]x + [1+3r]x^2 - [1+4r]x^3 + … ), r = 19/21. The sequence
 *            satisfies a linear recurrence of order 21 for n ≥ 23: a[n] =
 *            -∑_{i=1}^{21} g[i] a[n-i] , where g = [1, -1, 1, -1, 1, -1, 1,
 *            -1, 1, -1, 0, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1]...
 * Range:     i8 n <= 94, i16 n <= 190, i32 n <= 398, i64 n <= 789,
 *            i128 n <= 1573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Coefficients of the denominator polynomial of the rational generating
   function.  Index i corresponds to the coefficient of x^i. */
static const seq_t den_coeff[22] = {
    1,  /* g[0] */
   -1,  /* g[1] */
    1,  /* g[2] */
   -1,  /* g[3] */
    1,  /* g[4] */
   -1,  /* g[5] */
    1,  /* g[6] */
   -1,  /* g[7] */
    1,  /* g[8] */
   -1,  /* g[9] */
    0,  /* g[10] */
   -1,  /* g[11] */
    1,  /* g[12] */
   -1,  /* g[13] */
    1,  /* g[14] */
   -1,  /* g[15] */
    1,  /* g[16] */
   -1,  /* g[17] */
    1,  /* g[18] */
   -1,  /* g[19] */
    1,  /* g[20] */
    1   /* g[21] */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoized values */
    static idx_t cache_len = 0;       /* number of stored terms */

    /* Ensure the cache can hold up to index n. */
    idx_t required = n + 1;
    if (required > cache_len) {
        /* Allocate or grow the cache. */
        cache = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        /* Initialise the first term if needed. */
        if (cache_len == 0) {
            cache[0] = 1;          /* a(0) = 1 */
            cache_len = 1;
        }

        /* Compute new terms up to index n. */
        for (idx_t m = cache_len; m < required; ++m) {
            if (m <= 22) {          /* direct convolution for first 23 terms */
                seq_t sum = 0;
                for (idx_t k = 1; k <= m; ++k) {
                    seq_t coeff = ((seq_t)19 * k + 40) / 21;   /* floor((19k+40)/21) */
                    if (k & 1) coeff = -coeff;                /* (-1)^k */
                    sum += coeff * cache[m - k];
                }
                cache[m] = -sum;
            } else {                 /* linear recurrence for m ≥ 23 */
                seq_t sum = 0;
                for (int i = 1; i <= 21; ++i)
                    sum += den_coeff[i] * cache[m - i];
                cache[m] = -sum;
            }
        }
        cache_len = required;
    }

    return cache[n];
}
