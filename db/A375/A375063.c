/*
 * A375063  Expansion of 1 / Sum_{k in Z} x^k / (1 - x^(5*k+2)).
 * https://oeis.org/A375063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1 - x^{5k-1}) * (1 -
 *            x^{5k-4}) / (1 - x^{5k})^2 This is implemented by multiplying
 *            successively by each factor. The algorithm runs in O(n²) time
 *            with a small constant factor, suitable for n up to about 10⁴
 * Range:     i8 n <= 29, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate or resize an array to hold coefficients up to n. */
    static seq_t *coef = NULL;
    static idx_t size = 0;

    if (n + 1 > size) {
        free(coef);
        coef = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!coef) return 0;          /* allocation failure – unlikely on test system */
        size = n + 1;
    }

    /* Initialise the array: coefficient of x⁰ is 1, others 0. */
    coef[0] = 1;
    for (idx_t i = 1; i <= n; ++i) coef[i] = 0;

    /* Apply factors for k = 1, 2, … while any part of the factor
       operates inside the range [0, n]. */
    for (idx_t k = 1; ; ++k) {
        idx_t a1 = 5 * k - 1;   /* exponent for (1 - x^{5k-1}) */
        idx_t a2 = 5 * k - 4;   /* exponent for (1 - x^{5k-4}) */
        idx_t a0 = 5 * k;       /* exponent for (1 - x^{5k}) in denominator */

        if (a1 > n && a2 > n && a0 > n) break;

        /* Multiply by (1 - x^{a1}) if it applies.  Use descending loop
           to keep old values on the RHS. */
        if (a1 <= n) {
            for (idx_t i = n; i >= a1; --i) {
                coef[i] -= coef[i - a1];
                if (i == a1) break; /* prevent unsigned wrap‑around */
            }
        }

        /* Multiply by (1 - x^{a2}) if it applies. */
        if (a2 <= n) {
            for (idx_t i = n; i >= a2; --i) {
                coef[i] -= coef[i - a2];
                if (i == a2) break;
            }
        }

        /* Multiply by (1 - x^{a0})^{-2} = (1 - x^{a0})^{-1} twice.
           Each inverse is implemented as a prefix sum over each residue
           class modulo a0. */
        if (a0 <= n) {
            for (idx_t reps = 0; reps < 2; ++reps) {
                for (idx_t r = 0; r < a0; ++r) {
                    seq_t acc = 0;
                    for (idx_t i = r; i <= n; i += a0) {
                        acc += coef[i];
                        coef[i] = acc;
                    }
                }
            }
        }
    }

    return coef[n];
}
