/*
 * A396651  Triangle read by rows: the coefficients of the polynomials P(n, x)
 *          = x * P(n-1, x) + P(n-2, x), where P(0, x) = 1 and P(1, x) = 4*x,
 *          ordered by ascending powers.
 * https://oeis.org/A396651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattened triangle of coefficients of the *
 *            polynomials P(n,x) defined by * P(0,x) = 1, * P(1,x) = 4*x, *
 *            P(n,x) = x*P(n-1,x) + P(n-2,x) for n >= 2. * The sequence is
 *            obtained by writing each row of coefficients * (from lowest to
 *            highest power) and concatenating the rows
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    /* r is the last row index with its first element <= n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) r++;
    r--;                            /* now (r*(r+1))/2 <= n */
    idx_t start = r * (r + 1) / 2;   /* index of first element in row r */
    idx_t k = n - start;             /* column within the row */

    /* Handle the first two rows explicitly */
    if (r == 0) return (seq_t)1;                 /* row 0: [1]               */
    if (r == 1) return k == 0 ? (seq_t)0 : (seq_t)4; /* row 1: [0,4]           */

    /* Allocate row 0 (P(0,x)) and row 1 (P(1,x)) */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t));
    prev2[0] = (seq_t)1;                          /* P(0,x) */

    seq_t *prev1 = (seq_t *)malloc(2 * sizeof(seq_t));
    prev1[0] = (seq_t)0;
    prev1[1] = (seq_t)4;                          /* P(1,x) */

    /* Build rows up to r using the recurrence
     *   coeff(n,k) = coeff(n-1,k-1) + coeff(n-2,k)
     * with out‑of‑range coefficients treated as 0. */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t *newrow = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        for (idx_t j = 0; j <= i; ++j) {
            seq_t a = (j > 0) ? prev1[j - 1] : (seq_t)0;
            seq_t b = (j <= i - 2) ? prev2[j] : (seq_t)0;
            newrow[j] = a + b;
        }
        free(prev2);          /* free the oldest row */
        prev2 = prev1;        /* shift rows */
        prev1 = newrow;
    }

    seq_t result = prev1[k];
    free(prev2);
    free(prev1);
    return result;
}
