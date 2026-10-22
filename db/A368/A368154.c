/*
 * A368154  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 3*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - 3*x - x^2.
 * https://oeis.org/A368154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368154
 *
 *  Sequence constructed from the coefficient array T(n,k) of the strong
 *  divisibility sequence of polynomials:
 *
 *      p(1,x) = 1
 *      p(2,x) = 1 + 3*x                            (u = p(2,x))
 *      p(n,x) = u*p(n-1,x) + v*p(n-2,x)  (n>=3)
 *
 *  where   v = 1 - 3*x - x^2.
 *
 *  Let T(n,k) be the coefficient of x^k in p(n,x) (0 ≤ k < n).  The
 *  recurrence for the coefficients is
 *
 *      T(n,k) = T(n-1,k) + 3*T(n-1,k-1)
 *              + T(n-2,k) - 3*T(n-2,k-1) - T(n-2,k-2)
 *
 *  with base values
 *
 *      T(1,0) = 1
 *      T(2,0) = 1,   T(2,1) = 3
 *
 *  The sequence a(n) is obtained by reading the triangle by rows:
 *
 *      a(1)   = T(1,0)
 *      a(2)   = T(2,0)  a(3)   = T(2,1)
 *      a(4)   = T(3,0)  a(5)   = T(3,1)  a(6)   = T(3,2)
 *      …
 *
 *  For an index n ≥ 1, let r be the unique positive integer with
 *  r*(r+1)/2 ≥ n.  Then the coefficient for a(n) is
 *
 *      k = n - (r-1)*r/2 - 1      (zero‑based index in row r)
 *      a(n) = T(r,k)
 *
 *  The following implementation computes T(r,k) by iterating the
 *  recurrence.  It works for any int type provided for seq_t and for
 *  idx_t (unsigned 64‑bit).  It does not memoise across calls, but
 *  its O(r²) cost is negligible for r ≤ 140 (n ≤ 10 000).
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build.
 *   seq_t: signed integer of arbitrary width (or arbitrary‑precision class)
 *   idx_t: unsigned integer (uint64_t)
 */

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;        /* undefined for n <= 0 */

    /* Find the row r containing the n-th term. */
    idx_t r = 1;
    while (r * (r + 1U) / 2U < n)
        ++r;

    /* Index of the coefficient inside the row (zero‑based). */
    idx_t previous_terms = (r - 1U) * r / 2U;
    idx_t k = n - previous_terms - 1U;        /* 0 ≤ k < r */

    /* Handle the trivial first rows directly. */
    if (r == 1)                  /* row 1: [1] */
        return (seq_t)1;
    if (r == 2) {
        return (k == 0U) ? (seq_t)1 : (seq_t)3;
    }

    /* Allocate arrays for the two most recent rows.  Length r is enough
     * for all rows up to r. */
    size_t len = (size_t)r;
    seq_t *prev2 = (seq_t *)calloc(len, sizeof(seq_t));   /* row n-2 */
    seq_t *prev1 = (seq_t *)calloc(len, sizeof(seq_t));   /* row n-1 */

    if (!prev2 || !prev1) {
        /* Allocation failed – clean up and return 0. */
        free(prev2); free(prev1);
        return (seq_t)0;
    }

    /* Initialise rows 1 and 2. */
    prev2[0] = (seq_t)1;                     /* p(1,x) = 1          */
    prev1[0] = (seq_t)1;                     /* p(2,x) coefficient of x^0 */
    prev1[1] = (seq_t)3;                     /* p(2,x) coefficient of x^1 */

    /* Compute rows 3 … r iteratively. */
    for (idx_t row = 3U; row <= r; ++row) {
        /* Current row will have indices 0 … row-1. */
        seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
        if (!curr) {
            free(prev2); free(prev1);
            return (seq_t)0;
        }

        idx_t max_k = row;                    /* number of coefficients in current row */
        for (idx_t t = 0U; t < max_k; ++t) {
            seq_t val = (seq_t)0;

            /* Contributions from row n-1 (prev1). */
            if (t < row - 1U)
                val += prev1[t];
            if (t > 0U)
                val += (seq_t)3 * prev1[t - 1U];

            /* Contributions from row n-2 (prev2). */
            if (t < row - 2U)
                val += prev2[t];
            if (t > 0U)
                val -= (seq_t)3 * prev2[t - 1U];
            if (t > 1U)
                val -= prev2[t - 2U];

            curr[t] = val;
        }

        /* Re‑cycle the arrays. */
        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* The required coefficient is now in prev1[k]. */
    seq_t result = prev1[k];

    /* Clean up. */
    free(prev2);
    free(prev1);

    return result;
}
