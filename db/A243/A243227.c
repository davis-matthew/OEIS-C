/*
 * A243227  G.f.: Sum_{n>=0} n^(2*n) * x^n / (1 + n^2*x)^n.
 * https://oeis.org/A243227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑1)! * Stirling2(2*n+1, n) for n>0 a(0) = 1.
 *            Stirling2(m,k) is the Stirling number of the second kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   factorial, free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t stirling2_2n1_n(idx_t n)
{
    /* compute S(2*n+1, n) with DP using only two rows of width n+1 */
    idx_t const size = n + 1;
    seq_t *prev = seq_alloc_new_array(size);
    seq_t *curr = seq_alloc_new_array(size);

    /* Base: S(0,0)=1, other S(0,k)=0 */
    for (idx_t k = 0; k < size; ++k)
        prev[k] = (k == 0 ? (seq_t)1 : (seq_t)0);

    /* Iterate m = 1 .. 2*n+1 */
    for (idx_t m = 1; m <= 2 * n + 1; ++m) {
        curr[0] = 0;                     /* k=0 is always zero for m>0 */

        idx_t lim = (m < n ? m : n);     /* only need k<=min(m,n) */
        for (idx_t k = 1; k <= lim; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            seq_t term1 = prev[k];
            seq_t term2 = prev[k - 1];
            curr[k] = term1 * (seq_t)k + term2;
        }
        /* k>lim remain zero */
        for (idx_t k = lim + 1; k < size; ++k)
            curr[k] = 0;

        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[n];
    free_seq_array(prev);
    free_seq_array(curr);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t fact = factorial(n - 1);          /* (n-1)!  */
    seq_t stir = stirling2_2n1_n(n);         /* S(2n+1, n) */
    return fact * stir;                     /* a(n) = (n-1)! * S(2n+1,n) */
}
