/*
 * A286131  Expansion of q^(-1/2) * eta(q) * eta(q^30) * eta(q^35) * eta(q^42)
 *          in powers of q.
 * https://oeis.org/A286131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^4 * * Prod_{k>0} (1 - x^k)(1 -
 *            x^(30k))(1 - x^(35k))(1 - x^(42k)) in the * expansion in powers
 *            of x. The definition in the OEIS is * a(n) = coefficient of
 *            q^(n-4) in * eta(q)*eta(q^30)*eta(q^35)*eta(q^42) * q^(-1/2)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *coeff = NULL;          /* array of coefficients up to current max index */
static idx_t coeff_n = 0;            /* largest index currently stored in coeff[] */

/* Recompute the entire product up to index N (inclusive).  N is the highest
   index of the coefficient array before the x^4 shift.  After this
   function, coeff[i] holds the coefficient of x^i in
   Prod (1 – x^k)(1 – x^(30k))(1 – x^(35k))(1 – x^(42k)). */
static void recompute_coeffs(idx_t N)
{
    /* free old memory if any */
    if (coeff != NULL)
        free(coeff);

    coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (coeff == NULL)
        return;                      /* out of memory – caller will get 0 */

    /* initialise all coefficients to 0, except the constant term 1 */
    for (idx_t i = 0; i <= N; ++i)
        coeff[i] = 0;
    coeff[0] = 1;

    /* the four factors (1 - x^m) with m = k, 30k, 35k, 42k */
    const int factor[4] = {1, 30, 35, 42};
    for (int f = 0; f < 4; ++f) {
        int a = factor[f];
        for (idx_t k = 1; ; ++k) {
            idx_t m = (idx_t)a * k;
            if (m > N)
                break;
            /* multiply current polynomial by (1 - x^m) in place
               using a reverse loop to avoid reusing updated terms */
            for (idx_t i = N; i >= m; --i) {
                coeff[i] -= coeff[i - m];
            }
        }
    }
    coeff_n = N;
}

/* Returns a(n) for the sequence A286131.  The function keeps a cache
   of previously computed coefficients so that repeated calls are
   efficient. */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                      /* offset 0: first non‑zero term at n = 4 */

    idx_t target = n - 4;              /* index in the unshifted product */
    if (coeff == NULL || target > coeff_n) {
        recompute_coeffs(target);
    }
    return coeff[target];
}
