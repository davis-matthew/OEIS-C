/*
 * A371025  Triangle read by rows: T(n, k) = 2^n*Sum_{j=0..k} (-1)^(k -
 *          j)*binomial(k, j)* Pochhammer(j/2, n).
 * https://oeis.org/A371025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows.
   T(n,k) = k*T(n-1,k-1) + (2*n-2+k)*T(n-1,k)   (1 <= k <= n-1)
   T(n,0) = 0   for n > 0
   T(n,n) = n!  for n >= 0
   a(n) is the flattened triangle (row by row), offset 0.
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=3, a(5)=2, ...  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r and column k such that a(n) = T(r,k) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    if (r == 0)            /* a(0) = 1 */
        return 1;

    /* allocate temporary arrays for two consecutive rows */
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !cur) {
        if (prev) free(prev);
        if (cur)  free(cur);
        return 0;          /* on allocation failure return 0 */
    }

    /* Base row n = 0 */
    prev[0] = 1;

    seq_t fact = 1;        /* factorial accumulator for n! */

    for (idx_t i = 1; i <= r; ++i) {
        cur[0] = 0;        /* T(i,0) = 0 for i > 0 */

        /* compute interior elements using recurrence (1) */
        for (idx_t j = 1; j < i; ++j) {
            seq_t term1 = (seq_t)j * prev[j - 1];
            seq_t term2 = (seq_t)(2 * i - 2 + j) * prev[j];
            cur[j] = term1 + term2;
        }

        /* last element T(i,i) = i! */
        fact = fact * (seq_t)i;
        cur[i] = fact;

        if (i == r) {          /* we have reached desired row */
            seq_t result = cur[k];
            free(prev);
            free(cur);
            return result;
        }

        /* prepare for next iteration: swap arrays */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    /* Should never reach here */
    free(prev);
    free(cur);
    return 0;
}
