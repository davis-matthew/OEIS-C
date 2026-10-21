/*
 * A257613  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 2*x + 4.
 * https://oeis.org/A257613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle read by rows defined by
 *            T(0,0) = 1, T(n,0) = 4 * T(n-1,0) // T(n,0)=4^n T(n,k) = (2*k +
 *            4) * T(n-1,k) + (2*(n-k) + 4) * T(n-1,k-1) for k > 0, with
 *            T(n,k) = 0 when k > n. For a given n the row r and column k are
 *            r = floor((sqrt(8*n+1)-1)/2) k = n - r*(r+1)/2...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 47, u64 n <= 140, u128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column k of the nth term (row‑major order) */
    idx_t r = 0;
    idx_t remaining = n;
    while (remaining >= r + 1) {
        remaining -= r + 1;
        r++;
    }
    idx_t k = remaining;                   /* 0‑based column within the row */

    if (r == 0) {                           /* row 0 contains only 1 */
        return (seq_t)1;
    }

    /* Allocate two consecutive rows of the triangle */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {                   /* allocation failure */
        return (seq_t)0;                    /* not expected in tests */
    }

    /* Row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; i++) {
        /* k = 0 case: T(i,0) = 4 * T(i-1,0) */
        curr[0] = (seq_t)4 * prev[0];

        /* k >= 1 */
        for (idx_t j = 1; j <= i; j++) {
            seq_t term1 = 0;
            if (j <= i - 1) { /* T(i-1,j) exists only if j <= i-1 */
                term1 = (seq_t)(2 * j + 4) * prev[j];
            }
            seq_t term2 = (seq_t)(2 * (i - j) + 4) * prev[j - 1];
            curr[j] = term1 + term2;
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
