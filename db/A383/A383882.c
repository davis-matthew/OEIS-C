/*
 * A383882  a(n) = [x^n] Product_{k=1..4*n} 1/(1 - k*x).
 * https://oeis.org/A383882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(5*n,4*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = [x^n] Product_{k=1..4*n} 1/(1 - k*x) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Number of rows and columns for Stirling recursion */
    const idx_t N = 5 * n;           /* row index (m)   */
    const idx_t K = 4 * n;           /* column index (k)*/

    /* Allocate arrays for two DP rows; each of size K+1 */
    seq_t *prev = (seq_t *)calloc((size_t)(K + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(K + 1), sizeof(seq_t));
    if (!prev || !curr) {          /* safe exit on allocation failure */
        return (seq_t)0;
    }

    prev[0] = (seq_t)1;             /* S(0,0) = 1 */

    for (idx_t m = 1; m <= N; ++m) {
        /* B(.,.) row *needs* indices up to min(m, K) */
        const idx_t maxk = (m < K) ? m : K;
        curr[0] = (seq_t)0;        /* S(m,0) = 0 for m>0 */

        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t val = prev[k] * (seq_t)k; /* k * S(m-1, k) */
            val += prev[k - 1];            /* + S(m-1, k-1)  */
            curr[k] = val;
        }

        /* For k > maxk and <= K, values stay 0 (implicit). */
        /* Copy current row to prev for next iteration. */
        for (idx_t k = 0; k <= maxk; ++k)
            prev[k] = curr[k];
    }

    seq_t result = prev[K];          /* S(5n,4n) */

    free(prev);
    free(curr);
    return result;
}
