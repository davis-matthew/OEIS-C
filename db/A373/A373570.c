/*
 * A373570  Triangle read by rows: Coefficients of the polynomials S1(n, x) *
 *          EP(n, x), where S1 denote the unsigned Stirling cycle polynomials
 *          A132393 and EP the Eulerian polynomials A173018.
 * https://oeis.org/A373570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 34, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A373570: a(n) is the nth element of the triangle read by
   rows of the coefficients of S1(n, x) * EP(n, x) where
   S1 are unsigned Stirling cycle polynomials (A132393) and EP are Eulerian
   polynomials (A173018).  For n ≥ 0, a(n) is obtained by flattening the rows:
   row 0: [1]
   row r (r ≥ 1): 2 r coefficients of the product polynomial.  The function
   determines the row r and position pos for the given n, builds the
   required Stirling and Eulerian rows, and returns the coefficient
   c_pos. */

#include <stdint.h>
#include <stdlib.h>

/* Sequence value type and index type are provided by the build. */
extern seq_t factorial(idx_t n);      /* only for compliance if needed */
extern size_t digit_len(seq_t x);      /* only for compliance if needed */

seq_t a(idx_t n)
{
    /* Special case for n = 0.  Row 0 has a single element 1. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Determine the row number r (r ≥ 1) and the position pos within that
       row for the given index n.  The total number of terms before row r
       is T(r) = r*(r-1) + 1.  The number of terms in row r is 2r.       */
    const idx_t m = n - (idx_t)1;          /* n-1  (non‑negative) */

    idx_t r = 1;
    while (m >= r * (r + (idx_t)1)) {
        ++r;
    }

    const idx_t start = r * (r - (idx_t)1) + (idx_t)1;  /* first index of row r */
    const idx_t pos   = n - start;                     /* 0‑based position in row */

    /* ---------- Compute unsigned Stirling numbers of the first kind
       for n = r : S1(r,k), 0 ≤ k ≤ r ----------
    */
    seq_t *st = (seq_t *)calloc((size_t)(r + (idx_t)1), sizeof(seq_t));
    if (!st) {
        return (seq_t)0;
    }
    st[0] = (seq_t)1;               /* S1(0,0) = 1 */

    for (idx_t cur = 1; cur <= r; ++cur) {
        /* Update st[k] for k = cur … 1, using the recurrence
           S1(cur,k) = S1(cur-1,k-1) + (cur-1)*S1(cur-1,k).            */
        for (idx_t k = cur; k >= 1; --k) {
            st[k] = st[k - (idx_t)1] + (cur - (idx_t)1) * st[k];
        }
        st[0] = (seq_t)0;           /* S1(cur,0) = 0 for cur ≥ 1 */
    }

    /* ---------- Compute Eulerian numbers A(r,k), 0 ≤ k < r ----------
       Using the recurrence:
         A(n,k) = (k+1)*A(n-1,k) + (n-k)*A(n-1,k-1)   (A(1,0)=1)     */
    seq_t *e = NULL;
    if (r >= (idx_t)1) {
        e = (seq_t *)malloc((size_t)r * sizeof(seq_t));
        if (!e) {
            free(st);
            return (seq_t)0;
        }
        e[0] = (seq_t)1;                   /* A(1,0) = 1 */

        for (idx_t cur = 2; cur <= r; ++cur) {
            seq_t *newe = (seq_t *)malloc((size_t)cur * sizeof(seq_t));
            if (!newe) {
                free(e);
                free(st);
                return (seq_t)0;
            }
            for (idx_t k = 0; k < cur; ++k) {
                seq_t term1 = (k < cur - (idx_t)1) ? ((seq_t)(k + (idx_t)1)) * e[k] : (seq_t)0;
                seq_t term2 = (k > (idx_t)0) ? ((seq_t)(cur - k)) * e[k - (idx_t)1] : (seq_t)0;
                newe[k] = term1 + term2;
            }
            free(e);
            e = newe;
        }
    }

    /* ---------- Compute the coefficient at position pos ----------
       The coefficient is c_pos = Σ_{i=0}^{r} S1(r,i) * A(r, pos-i)
       (with the convention that A(r, j)=0 if j<0 or j≥r).          */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= r && i <= pos; ++i) {
        const idx_t j = pos - i;
        if (j < r) {
            result += st[i] * e[j];
        }
    }

    free(st);
    free(e);
    return result;
}
