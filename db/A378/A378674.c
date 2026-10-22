/*
 * A378674  Triangle T(n,k) read by rows, where row n is a permutation of
 *          numbers 1 through n, such that if the deck of n cards is prepared
 *          in this order, and down-under dealing is used, then the resulting
 *          cards are put down in increasing order.
 * https://oeis.org/A378674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element read by rows from the triangle T(n,k).
 *            T(1,1)=1; for n>1, T(n,1)=1, T(n,2)=T(n-1,n-1)+1, and for k>2,
 *            T(n,k)=T(n-1,k-2)+1. For a given index n>0, let r be the
 *            smallest integer with r(r+1)/2 >= n (row number) and k = n -
 *            r(r-1)/2 (column). Then a(n)=T(r,k). The function below c...
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined for n=0, return 0 */ return (seq_t)0; }

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1U) / 2U) < n) ++r;

    /* Column index k (1-based) */
    idx_t k = n - r * (r - 1U) / 2U;

    /* Build rows up to r */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!prev) return (seq_t)0;  /* allocation failed */
    prev[0] = (seq_t)1;          /* row 1 */

    if (r > 1) {
        for (idx_t i = 2; i <= r; ++i) {
            size_t size_curr = (size_t)i;
            seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * size_curr);
            if (!curr) {
                free(prev);
                return (seq_t)0;  /* allocation failed */
            }
            curr[0] = (seq_t)1;                    /* T(i,1) = 1 */
            curr[1] = prev[i - 2U] + (seq_t)1;     /* T(i,2) = T(i-1,i-1)+1 */
            for (idx_t j = 2; j < i; ++j) {         /* k>2 case */
                curr[j] = prev[j - 2U] + (seq_t)1;
            }
            free(prev);
            prev = curr;
        }
    }

    seq_t result = prev[k - 1U];
    free(prev);
    return result;
}
