/*
 * A352284  a(n) is 2^(2*n) times the derivative of order 2*n of the logarithm
 *          of I_0(x) (the modified Bessel function of the first kind of order
 *          zero) evaluated at zero.
 * https://oeis.org/A352284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = C(2n,n) * (u_n / n) * u_1 = 1 * u_n = n − Σ_{k=1}^{n-1}
 *            u_k * C(n,k)^2 (n ≥ 2) * a(0) = 0 * This implementation uses
 *            lazy memoization; values up to the * largest n requested are
 *            stored in static arrays
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    static seq_t *u_cache  = NULL;   /* u_i values */
    static seq_t *a_cache  = NULL;   /* a_i values */
    static seq_t *c2_cache = NULL;   /* C(2i,i) values */
    static idx_t  allocated = 0;      /* allocated size (<= n+1) */

    if (n >= allocated) {
        idx_t new_size = n + 1;
        u_cache   = (seq_t *)realloc(u_cache,   new_size * sizeof(seq_t));
        a_cache   = (seq_t *)realloc(a_cache,   new_size * sizeof(seq_t));
        c2_cache  = (seq_t *)realloc(c2_cache,  new_size * sizeof(seq_t));
        idx_t old_size = allocated;

        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) {
                u_cache[0]  = (seq_t)0;
                a_cache[0]  = (seq_t)0;
                c2_cache[0] = (seq_t)1;   /* C(0,0) = 1 */
                continue;
            }

            /* Compute u_i via the recurrence */
            seq_t bin = (seq_t)1;   /* C(i,0) */
            seq_t sum = (seq_t)0;

            for (idx_t k = 1; k < i; ++k) {
                bin = bin * (seq_t)(i - (k - 1));
                bin = bin / (seq_t)k;
                seq_t bin2 = bin * bin;
                sum += u_cache[k] * bin2;
            }

            seq_t ui = (seq_t)i - sum;
            u_cache[i] = ui;

            /* Compute central binomial coefficient C(2i,i) */
            if (i == 1) {
                c2_cache[1] = (seq_t)2;
            } else {
                seq_t prev = c2_cache[i - 1];
                c2_cache[i] = prev * ((seq_t)(4 * i - 2));
                c2_cache[i] = c2_cache[i] / (seq_t)i;
            }

            /* a_i = C(2i,i) * (ui / i) */
            seq_t ai = c2_cache[i] * (ui / (seq_t)i);
            a_cache[i] = ai;
        }

        allocated = new_size;
    }
    return a_cache[n];
}
