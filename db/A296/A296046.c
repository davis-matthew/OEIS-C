/*
 * A296046  Expansion of Product_{k>=1} ((1 - x^(2*k))/(1 - x^(2*k-1)))^k.
 * https://oeis.org/A296046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Product_{k≥1} ((1 - x^(2k))/(1 -
 *            x^(2k-1)))^k. Recurrence derived from the logarithm: a(0) = 1
 *            for n ≥ 1: S(n) = Σ_{d|n} d * e_d where e_d = d/2 if d is even
 *            -(d+1)/2 if d is odd a(n) = -(1/n) * Σ_{k=1}^n S(k) * a(n-k) The
 *            coefficients S(k) depend only on the exponent...
 * Range:     i8 n <= 14, i16 n <= 47, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term (0-indexed) of OEIS A296046. */
seq_t a(idx_t n)
{
    static idx_t max_n = 0;     /* maximum n computed so far */
    static seq_t *a_val = NULL; /* cached sequence values */
    static seq_t *Scoef = NULL; /* cached S(k) coefficients */

    /* Handle the trivial first term separately to avoid uninitialised memory. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate/extend storage if a larger n is requested. */
    if (n > max_n) {
        idx_t new_size = n + 1;

        a_val  = (seq_t *)realloc(a_val,  new_size * sizeof(seq_t));
        Scoef  = (seq_t *)realloc(Scoef, new_size * sizeof(seq_t));

        /* Initialise the first value of the sequence. */
        if (max_n == 0) {
            a_val[0] = (seq_t)1;
        }

        /* Compute S(k) for all new indices.  S(k) = Σ_{d|k} d*e_d. */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t s = (seq_t)0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    /* Current divisor d */
                    seq_t ed;
                    if (d & 1) {                 /* odd */
                        ed = -(seq_t)((d + 1) / 2);
                    } else {                     /* even */
                        ed = (seq_t)(d / 2);
                    }
                    s += (seq_t)d * ed;

                    /* Complementary divisor k/d, if distinct */
                    idx_t d2 = k / d;
                    if (d2 != d) {
                        seq_t ed2;
                        if (d2 & 1) {
                            ed2 = -(seq_t)((d2 + 1) / 2);
                        } else {
                            ed2 = (seq_t)(d2 / 2);
                        }
                        s += (seq_t)d2 * ed2;
                    }
                }
            }
            Scoef[k] = s;
        }

        /* Compute new sequence values using the recurrence. */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                sum += Scoef[k] * a_val[m - k];
            }
            a_val[m] = -(sum) / (seq_t)m;   /* exact division */
        }

        max_n = n;
    }

    return a_val[n];
}
