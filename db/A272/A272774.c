/*
 * A272774  Triangle read by rows, T(n, k) = (n-k)*T(n-1, k-1) + k*T(n-1, k+1)
 *          except that T(n, k) = 0 if k<1 or k>n and T(n, n) = 1, for n>=0
 *          and 0<=k<=n.
 * https://oeis.org/A272774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle recurrence: T(n,k) = (n-k)*T(n-1,k-1) + k*T(n-1,k+1),  
   boundary T(n,n)=1, and T(n,k)=0 for k<0 or k>n. 
   a(n) is the nth entry when the triangle is read row‑by‑row starting at n=0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Base triangle row 0: T(0,0)=1 */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        for (idx_t j = 0; j <= i; ++j) {
            if (j == i) {
                curr[j] = (seq_t)1;
            } else {
                seq_t val = (seq_t)0;
                if (j > 0)  val += (seq_t)(i - j) * prev[j - 1];
                if (j + 1 <= i - 1) val += (seq_t)j * prev[j + 1];
                curr[j] = val;
            }
        }
        free(prev);
        prev = curr;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
