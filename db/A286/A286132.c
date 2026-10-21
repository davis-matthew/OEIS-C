/*
 * A286132  Expansion of q^(-1/2) * eta(q^3) * eta(q^10) * eta(q^14) *
 *          eta(q^105) in powers of q.
 * https://oeis.org/A286132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * x^5 * Π_{k>0} (1 - x^{3k})(1 -
 *            x^{10k})(1 - x^{14k})(1 - x^{105k})
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *coeff = NULL;       /* memoised coefficient array */
static idx_t coeff_max = 0;       /* largest exponent computed so far */
static int  coeff_init = 0;       /* flag: has coeff been initialized? */

/* ------------------------------------------------------------------
 * compute_coeffs(maxE)
 *   Compute all coefficients from exponent 0..maxE for the product
 *   Π_{m∈{3,10,14,105}} Π_{k≥1}(1 - x^{m k}) and store them in coeff[].
 *   The routine allocates a new array and discards any previous one.
 * ------------------------------------------------------------------ */
static void compute_coeffs(idx_t maxE)
{
    size_t N = (size_t)maxE + 1;
    seq_t *newcoeff = (seq_t*)calloc(N, sizeof(seq_t));
    if (!newcoeff) abort();

    newcoeff[0] = 1;                 /* start with the constant term 1 */

    /* The four moduli appearing in the product. */
    const int ms[4] = { 3, 10, 14, 105 };

    for (int mi = 0; mi < 4; ++mi) {
        int m = ms[mi];
        for (idx_t k = 1; (idx_t)k * m <= maxE; ++k) {
            idx_t e   = (idx_t)k * m;     /* exponent of the factor (1 - x^e) */
            size_t ee = (size_t)e;

            /* Update coefficients in reverse order so that each
             * factor is applied once.  This is equivalent to
             * multiplying the current polynomial by (1 - x^e). */
            for (size_t i = maxE; ; --i) {
                newcoeff[i] -= newcoeff[i - ee];
                if (i == ee) break;
            }
        }
    }

    free(coeff);
    coeff = newcoeff;
    coeff_max = maxE;
    coeff_init = 1;
}

/* ------------------------------------------------------------------
 * a(n)
 *   Return the n-th term of OEIS A286132.  The function works for
 *   arbitrary n ≥ 0 regardless of the order in which it is called.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 5) return 0;                 /* coefficients before the offset */

    idx_t e = n - 5;                     /* exponent needed from the product */
    if (!coeff_init || e > coeff_max) {
        compute_coeffs(e);
    }
    return coeff[(size_t)e];
}
