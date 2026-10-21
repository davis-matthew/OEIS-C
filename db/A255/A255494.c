/*
 * A255494  Triangle read by rows: coefficients of numerator of generating
 *          functions for powers of Pell numbers.
 * https://oeis.org/A255494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = P(n-k+1)*T(n-1,k-1) + P(k+1)*T(n-1,k), with
 *            T(n,0)=T(n,n)=1 and symmetry T(n,k)=T(n,n-k). a(n) = T(r,c)
 *            where r = floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 109,
 *            u128 n <= 217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the nth term of OEIS A255494 */
seq_t a(idx_t n)
{
    /* Find the row r containing the index n */
    idx_t r = 0;
    idx_t sum = 0;           /* sum = r*(r+1)/2 after the loop */
    while (sum + r + 1 <= n) {
        ++r;
        sum += r;
    }
    idx_t c = n - sum;       /* column within row r */

    /* Base row 0 is simply 1 */
    if (r == 0) {
        return 1;
    }

    /* Compute Pell numbers P[0..r+1] */
    idx_t psize = r + 2;                         /* need indices up to r+1 */
    seq_t *pell = (seq_t *)malloc(psize * sizeof(seq_t));
    if (!pell) return 0;                         /* memory failure */
    pell[0] = 0;
    pell[1] = 1;
    for (idx_t i = 2; i < psize; ++i) {
        pell[i] = ((seq_t)2 * pell[i-1]) + pell[i-2];
    }

    /* Allocate two rows of max size r+1 */
    idx_t rowsz = r + 1;
    seq_t *prev = (seq_t *)malloc(rowsz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(rowsz * sizeof(seq_t));
    if (!prev || !curr) {
        free(pell);
        free(prev);
        free(curr);
        return 0;
    }

    /* Row 0 */
    prev[0] = 1;

    /* Build rows 1 .. r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = 1;
        curr[i] = 1;
        for (idx_t k = 1; k < i; ++k) {
            /* T(i,k) = P(i-k+1)*prev[k-1] + P(k+1)*prev[k] */
            curr[k] = pell[i - k + 1] * prev[k-1] + pell[k + 1] * prev[k];
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    /* Clean up */
    free(pell);
    free(prev);
    free(curr);

    return result;
}
