/*
 * A368157  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 2*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 + 2*x^2.
 * https://oeis.org/A368157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p(1,x)=1, p(2,x)=1+2*x;
   p(n,x)=(1+2*x)*p(n-1,x)+(1+2*x^2)*p(n-2,x).
   a(n) is the nth coefficient when the rows of the triangular array
   T(n,k) are read left‑to‑right, starting with a(1)=1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset is 1 */

    /* Find the row r such that r(r+1)/2 >= n  */
    idx_t r = 1, cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t k = n - (cum - r);       /* 1‑based position in row r */

    /* Special cases for the first two rows */
    if (r == 1) return 1;
    if (r == 2) return (k == 1 ? (seq_t)1 : (seq_t)2);

    /* Allocate a few rows.  Indices 1..r are used; index 0 is ignored. */
    size_t cap = (size_t)r + 1;
    seq_t *prev2 = (seq_t*)calloc(cap, sizeof(seq_t));
    seq_t *prev1 = (seq_t*)calloc(cap, sizeof(seq_t));
    if (!prev2 || !prev1) return 0;  /* allocation failure */

    /* Base rows */
    prev2[1] = 1;          /* p(1,x) */
    prev1[1] = 1;          /* p(2,x) first coef */
    prev1[2] = 2;          /* p(2,x) second coef */

    /* Build rows 3 .. r */
    for (idx_t row = 3; row <= r; ++row) {
        seq_t *curr = (seq_t*)calloc((size_t)row + 1, sizeof(seq_t));
        if (!curr) { free(prev2); free(prev1); return 0; }

        for (idx_t pos = 1; pos <= row; ++pos) {
            seq_t val = 0;

            /* u = 1 + 2*x term acting on prev1 (p(n-1,x)) */
            if (pos <= row - 1) val += prev1[pos];
            if (pos >= 2)        val += (seq_t)2 * prev1[pos - 1];

            /* v = 1 + 2*x^2 term acting on prev2 (p(n-2,x)) */
            if (pos <= row - 2) val += prev2[pos];
            if (pos >= 3)        val += (seq_t)2 * prev2[pos - 2];

            curr[pos] = val;
        }

        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    seq_t result = prev1[k];
    free(prev2);
    free(prev1);
    return result;
}
