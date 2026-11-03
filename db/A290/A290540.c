/*
 * A290540  Determinant of circulant matrix of order 10 with entries in the
 *          first row that are (-1)^(j-1)*Sum_{k>=0} (-1)^k*binomial(n,
 *          10*k+j-1), for j=1..10.
 * https://oeis.org/A290540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 9, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = determinant of a 10×10 circulant matrix whose first row
 *   has entries c_j = (-1)^(j-1) * K_j(n)   (j = 1..10),
 *   where K_j(n) = Σ_{k≥0} (-1)^k * binomial(n, 10k + j - 1)
 *   (binomial is the standard binomial coefficient).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute the resultant of two integer polynomials
   G of degree dg (coefficients G[0..dg] in descending order)
   F of degree df (coefficients F[0..df] in descending order).
   Uses the Bareiss algorithm – exact division is always safe. */
static seq_t _resultant(const seq_t *G, size_t dg,
                        const seq_t *F, size_t df)
{
    if (dg == 0 && df == 0) return (seq_t)1;
    size_t sz = dg + df;
    /* Max degree in this problem is 4 (Φ) + 9 (f) = 13, so 25 is safe. */
    seq_t mat[25][25] = {0};

    /* First df rows: shifted copies of G */
    for (size_t i = 0; i < df; ++i) {
        for (size_t j = 0; j <= dg; ++j) {
            mat[i][i + j] = G[j];
        }
    }
    /* Next dg rows: shifted copies of F */
    for (size_t i = 0; i < dg; ++i) {
        for (size_t j = 0; j <= df; ++j) {
            mat[df + i][i + j] = F[j];
        }
    }

    int sign = 1;          /* track row swaps for sign */
    seq_t denom = (seq_t)1;/* previous pivot, initially 1 */

    for (size_t k = 0; k + 1 < sz; ++k) {
        /* Pivot search and possible swap */
        if (mat[k][k] == (seq_t)0) {
            size_t rr = k + 1;
            for (; rr < sz && mat[rr][k] == (seq_t)0; ++rr) {}
            if (rr == sz) return (seq_t)0;          /* determinant zero */
            for (size_t c = 0; c < sz; ++c) {
                seq_t tmp = mat[k][c];
                mat[k][c] = mat[rr][c];
                mat[rr][c] = tmp;
            }
            sign = -sign;
        }
        seq_t pivot = mat[k][k];
        for (size_t i = k + 1; i < sz; ++i) {
            for (size_t j = k + 1; j < sz; ++j) {
                /* (M[i][j]*pivot - M[i][k]*M[k][j]) / denom */
                mat[i][j] = (mat[i][j] * pivot - mat[i][k] * mat[k][j]) / denom;
            }
        }
        denom = pivot;
    }
    seq_t det = mat[sz - 1][sz - 1];
    return (sign == 1) ? det : -det;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* ---------- Compute the K_j(n) sums without storing all binomials ---------- */
    seq_t K[10] = {0};
    bool signPos[10] = {true, true, true, true, true, true, true, true, true, true};

    /* m = 0 term */
    K[0] += (seq_t)1;
    signPos[0] = !signPos[0];

    seq_t cur = (seq_t)1;          /* binom(n,0) */
    for (idx_t m = 1; m <= n; ++m) {
        /* Compute next binomial: binom(n,m) = binom(n,m-1) * (n-m+1)/m */
        cur = cur * ((seq_t)n - (seq_t)(m - 1));
        cur = cur / (seq_t)m;

        int j = (int)(m % 10);
        if (signPos[j]) K[j] += cur;
        else            K[j] -= cur;
        signPos[j] = !signPos[j];
    }

    /* ---------- Build the first-row coefficients c_j ---------- */
    seq_t c[10];
    for (int j = 0; j < 10; ++j) {
        c[j] = (j & 1) ? -K[j] : K[j];   /* (-1)^j factor */
    }

    /* ---------- Determine degree of f(x) = Σ c_j x^j ----------
       f is stored in descending order for the resultant routine. */
    int deg_f = 9;
    while (deg_f >= 0 && c[deg_f] == (seq_t)0) --deg_f;
    if (deg_f < 0) return (seq_t)0;   /* zero polynomial */

    seq_t f_desc[10];
    for (int i = 0; i <= deg_f; ++i) {
        f_desc[i] = c[deg_f - i];
    }
    size_t f_deg = (size_t)deg_f;

    /* ---------- Cyclotomic polynomials (descending coefficients) ----------
       Φ1(x) = x - 1
       Φ2(x) = x + 1
       Φ5(x) = x^4 + x^3 + x^2 + x + 1
       Φ10(x) = x^4 - x^3 + x^2 - x + 1
    */
    const seq_t phi1[]  = { (seq_t)1, (seq_t)-1 };
    const seq_t phi2[]  = { (seq_t)1, (seq_t)1 };
    const seq_t phi5[]  = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1 };
    const seq_t phi10[] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)-1, (seq_t)1 };

    /* ---------- Compute the product of resultants ----------
       Res(Φ1,f) * Res(Φ2,f) * Res(Φ5,f) * Res(Φ10,f)          */
    seq_t det = _resultant(phi1, 1, f_desc, f_deg);
    if (det == (seq_t)0) return (seq_t)0;
    det *= _resultant(phi2, 1, f_desc, f_deg);
    if (det == (seq_t)0) return (seq_t)0;
    det *= _resultant(phi5, 4, f_desc, f_deg);
    if (det == (seq_t)0) return (seq_t)0;
    det *= _resultant(phi10, 4, f_desc, f_deg);

    return det;
}
