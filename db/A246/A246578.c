/*
 * A246578  Expansion of g.f. (Product_{r>=1} (1 -
 *          x^r))*x^(k^2)/Product_{i=1..k} ((1-x^i)^2) with k=4.
 * https://oeis.org/A246578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in (∏_{r≥1}(1−x^r)) * x^16 /
 *            ∏_{i=1..4}(1−x^i)^2
 * Range:     i8 n <= 47, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(n) = 0 for n < 16 */
    if (n < 16) return 0;

    idx_t need = n - 16;           /* index in the series after the shift */

    /* static cache of computed terms up to max_needed */
    static idx_t max_cached = 0;
    static seq_t *c_arr = NULL;    /* coefficients of Q(x) = ∏_{i=5}^{∞} (1−x^i)  */
    static seq_t *f_arr = NULL;    /* coefficients of F(x) = Q(x)/R(x)            */

    /* recompute if we need a larger range or we have no cache yet */
    if (need > max_cached || c_arr == NULL) {
        /* free old cache */
        if (c_arr) free(c_arr);
        if (f_arr) free(f_arr);

        /* allocate new cache */
        c_arr = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        f_arr = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (!c_arr || !f_arr) return 0;   /* allocation failed */

        /* initialize to zero */
        for (idx_t i = 0; i <= need; ++i) {
            c_arr[i] = 0;
            f_arr[i] = 0;
        }
        c_arr[0] = 1;   /* product of nothing */

        /* build Q(x) = ∏_{i=5}^{need} (1−x^i) */
        for (idx_t i = 5; i <= need; ++i) {
            for (idx_t k = need; k >= i; --k) {
                c_arr[k] = c_arr[k] - c_arr[k - i];
            }
        }

        /* build f[m] = coefficient of x^m in F(x) = Q(x)/R(x) */
        for (idx_t m = 0; m <= need; ++m) {
            seq_t val = c_arr[m];       /* start with c_m */

            if (m >= 1) val = val + f_arr[m - 1];
            if (m >= 2) val = val + f_arr[m - 2];
            if (m >= 5) {
                val = val - f_arr[m - 5];
                val = val - f_arr[m - 5];   /* subtract twice */
            }
            if (m >= 8) val = val + f_arr[m - 8];
            if (m >= 9) val = val + f_arr[m - 9];
            if (m >= 10) val = val - f_arr[m - 10];

            f_arr[m] = val;
        }

        max_cached = need;
    }

    return f_arr[need];
}
