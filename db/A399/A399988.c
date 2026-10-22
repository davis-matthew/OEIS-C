/*
 * A399988  Irregular triangle read by rows: Sum_{m>=2} (-1)^m/A000217(m)^n =
 *          T(n,0) + T(n,1)*log(2) + Sum_{k=2..ceiling(n/2)}
 *          T(n,k)*zeta(2*k-1).
 * https://oeis.org/A399988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, easy, new
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 47, i64 n <= 154, i128 n <= 527,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A399988 – Irregular triangle read by rows.
 *
 * a(n) is the n-th coefficient in the flattening of the triangle whose
 * rows are defined by
 *
 *   T(r,0) = (-1)^(r-1) * [ 2^(r+1) * Σ_{j odd,1≤j≤r} C(2r-j-1,r-1)
 *                           – Σ_{i=0}^{r-1} C(r-1+i,r-1) * 2^i ]
 *
 *   T(r,1) = (-1)^r * 2^(r+1) * C(2r-2, r-1)
 *
 *   T(r,k) = (-1)^r * (2^(r+1)-2^(r+3-2k)) * C(2r-2k, r-1)      for k≥2
 *
 * The terms are listed row by row, so this function first determines
 * the row r and the position k within that row, then evaluates the
 * appropriate T(r,k) using the formulas above.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Cumulated number of terms up to row r (r ≥ 0).  C(0)=0. */
static idx_t cum_terms(idx_t r)
{
    if (r == 0)
        return 0;
    if ((r & 1) == 0) {                 /* even r = 2m */
        idx_t m = r >> 1;
        return m * m + 3 * m;
    } else {                            /* odd r = 2m-1 */
        idx_t m = (r + 1) >> 1;          /* m = (r+1)/2 */
        return m * m + 2 * m - 1;
    }
}

/* Main function to compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */

    /* Find row r such that cum_terms(r-1) < n ≤ cum_terms(r). */
    idx_t r = 1;
    while (cum_terms(r) < n)
        ++r;

    idx_t prev = cum_terms(r - 1);
    idx_t k = n - prev - 1;              /* position in row, 0‑based */

    /* ---------- T(r,0) ------------------------------------ */
    if (k == 0) {
        seq_t sumS1 = 0;
        for (idx_t j = 1; j <= r; j += 2) {
            sumS1 += binomial(2 * r - j - 1, r - 1);
        }

        seq_t sumS2 = 0;
        seq_t binom = 1;                 /* C(r-1+0, r-1) */
        seq_t pow2i = 1;
        for (idx_t i = 0; i < r; ++i) {
            sumS2 += binom * pow2i;
            /* next power of two */
            pow2i *= 2;
            if (i + 1 < r) {
                binom = binom * (seq_t)(r - 1 + i + 1) / (i + 1);
            }
        }

        seq_t term0 = pow2(r + 1) * sumS1 - sumS2;
        if ((r - 1) & 1)
            term0 = -term0;
        return term0;
    }

    /* ---------- T(r,1) ------------------------------------ */
    if (k == 1) {
        seq_t term1 = pow2(r + 1) * binomial(2 * r - 2, r - 1);
        if (r & 1)
            term1 = -term1;
        return term1;
    }

    /* ---------- T(r,k) for k ≥ 2 --------------------------- */
    seq_t termk = (pow2(r + 1) - pow2(r + 3 - 2 * k))
                  * binomial(2 * r - 2 * k, r - 1);
    if (r & 1)
        termk = -termk;
    return termk;
}
