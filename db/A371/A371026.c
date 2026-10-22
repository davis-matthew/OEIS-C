/*
 * A371026  Triangle read by rows: T(n, k) = 4^n*Sum_{j=0..k} (-1)^(k -
 *          j)*binomial(k, j)* Pochhammer(j/4, n).
 * https://oeis.org/A371026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(m,k) of the triangle read by rows * where m
 *            = floor((sqrt(8*n+1)-1)/2) and k = n - m*(m+1)/2. * Recurrence
 *            for the triangle: * T(0,0) = 1, * T(n,0) = 0 for n>0, * T(n,n) =
 *            n! , * T(n,k) = k*T(n‑1,k‑1) + (4*n‑4+k)*T(n‑1,k) (0<k<n)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n=0 */
    if (n == 0) return (seq_t)1;

    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    while (((m + 1) * (m + 2)) / 2 <= n) ++m;

    /* The position within the row */
    idx_t k = n - m * (m + 1) / 2;

    /* Allocate two rows of length m+1 */
    seq_t *prev = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!prev || !curr) { /* allocation failed – return 0 as a safe fallback */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Base row n = 0 */
    prev[0] = (seq_t)1;

    seq_t fact = (seq_t)1;   /* factorial accumulator */

    for (idx_t r = 1; r <= m; ++r) {
        curr[0] = (seq_t)0;                 /* T(r,0) = 0 for r>0 */
        fact = fact * (seq_t)r;              /* r! for the current row */

        /* Compute internal entries */
        for (idx_t j = 1; j < r; ++j) {
            /* term1 = j * prev[j-1] */
            seq_t term1 = (seq_t)j * prev[j - 1];
            /* term2 = (4*r - 4 + j) * prev[j] */
            seq_t term2 = ((seq_t)(4 * r - 4 + j)) * prev[j];
            curr[j] = term1 + term2;
        }

        /* Corner entry */
        curr[r] = fact;           /* T(r,r) = r! */

        if (r == m) {            /* desired row reached */
            seq_t result = curr[k];
            free(prev);
            free(curr);
            return result;
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Fallback (should never happen) */
    free(prev);
    free(curr);
    return (seq_t)0;
}
