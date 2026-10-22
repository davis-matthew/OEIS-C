/*
 * A357831  a(n) = Sum_{k=0..floor(n/3)} 2^k * |Stirling1(n,3*k)|.
 * https://oeis.org/A357831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * |Stirling1(n,3*k)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* Allocate space for two rows of the unsigned Stirling triangle. */
    seq_t *prev = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Initialize the first row: S(0,0)=1, all others 0. */
    for (idx_t i = 0; i <= n; ++i)
        prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    for (idx_t m = 1; m <= n; ++m) {

        /* Prepare current row: S(m,0)=0. */
        curr[0] = (seq_t)0;

        /* Ensure the extra element of the previous row is zero. */
        prev[m] = (seq_t)0;

        /* Compute the signed Stirling numbers of the first kind in their
           unsigned (absolute) form: S(m,k) = S(m-1,k-1) + (m-1)*S(m-1,k). */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = prev[k - 1];
            seq_t term2 = prev[k];
            seq_t mul    = term2 * (seq_t)(m - 1);
            curr[k] = term1 + mul;
        }

        /* If we've reached the requested row, compute the sum. */
        if (m == n) {
            seq_t sum = (seq_t)0;
            for (idx_t t = 0; t <= m; t += 3) {
                idx_t kk = t / 3;                 /* k = t/3 */
                seq_t power = pow2(kk);           /* 2^k */
                sum += power * curr[t];
            }
            result = sum;
        }

        /* Swap prev and curr for the next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    return result;
}
