/*
 * A293305  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of g.f. Product_{i>0} (1 + Sum_{j=1..k}
 *          (-1)^j*j*x^(j*i)).
 * https://oeis.org/A293305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   a(n) is obtained from the square array A(n,k) defined by the
 *            generating function ∏_{i>0} (1 + Σ_{j=1..k} (-1)^j·j·x^{j·i}).
 *            The array is read by antidiagonals: a(0)=A(0,0), a(1)=A(0,1),
 *            a(2)=A(1,0), a(3)=A(0,2), … This code precomputes the array for
 *            all indices up to diagonal 200 (enough for n≤10...
 * Range:     i8 n <= 425, i16 n <= 2336, i32 n <= 9869, i64 n <= 9869,
 *            i128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Maximum diagonal (row+column) that will be required.  For n≤10000
   this is 140, so 200 is a safe upper bound. */
#define MAX_D 200

/* Pre‑computed table: A[row][col] (both 0‑indexed) */
static seq_t A[MAX_D + 1][MAX_D + 1];
static bool initialized = false;

/* Build the table for all rows and columns up to MAX_D. */
static void build_table(void)
{
    for (idx_t col = 0; col <= MAX_D; ++col) {
        seq_t coeff[MAX_D + 1];
        seq_t oldcoeff[MAX_D + 1];
        seq_t newcoeff[MAX_D + 1];

        /* Start with the empty product: coefficient of x^0 is 1. */
        coeff[0] = (seq_t)1;
        for (idx_t n = 1; n <= MAX_D; ++n)
            coeff[n] = (seq_t)0;

        /* Multiply by each factor (1 + Σ_{j=1..col} (-1)^j·j·x^{j·i}) for i=1..MAX_D. */
        for (idx_t i = 1; i <= MAX_D; ++i) {
            /* Preserve the old coefficients for this round. */
            for (idx_t k = 0; k <= MAX_D; ++k)
                oldcoeff[k] = coeff[k];

            /* Start newcoeff as a copy of oldcoeff (choosing the '1' in each factor). */
            for (idx_t k = 0; k <= MAX_D; ++k)
                newcoeff[k] = oldcoeff[k];

            for (idx_t j = 1; j <= col; ++j) {
                idx_t exp = j * i;
                if (exp > MAX_D)
                    break;               /* Higher exponents don't affect coefficients ≤ MAX_D. */

                seq_t mult = (j & 1) ? -(seq_t)j : (seq_t)j;

                /* Convolve: add contributions of x^{exp}. */
                for (idx_t e = 0; e <= MAX_D - exp; ++e)
                    newcoeff[e + exp] += oldcoeff[e] * mult;
            }

            /* Update coeff for the next i. */
            for (idx_t k = 0; k <= MAX_D; ++k)
                coeff[k] = newcoeff[k];
        }

        /* Store the computed coefficients for this column. */
        for (idx_t n = 0; n <= MAX_D; ++n)
            A[n][col] = coeff[n];
    }
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_table();
        initialized = true;
    }

    /* Find diagonal d such that n lies on it: d(d+1)/2 ≤ n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while ((d * (d + 1)) / 2 <= n)
        ++d;
    --d;                                   /* now d satisfies the inequality. */

    idx_t offset = d * (d + 1) / 2;
    idx_t p = n - offset;                  /* position within the diagonal. */
    idx_t row = p;                         /* i */
    idx_t col = d - p;                     /* j */

    return A[row][col];
}
