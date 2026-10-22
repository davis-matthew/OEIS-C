/*
 * A378612  a(n) = Sum_{k=0..n} binomial(3*n+k-1,k) * binomial(n-1,n-k).
 * https://oeis.org/A378612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n + k -1, k) * binomial(n-1, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A378612) */

#include <stdint.h>   /* for uint64_t used in idx_t definition */

/* Forward declaration of binomial helper (provided elsewhere). */
seq_t binom(idx_t n, idx_t k);

/* Compute a(n) for any non-negative n. */
seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly. */
    if (n == 0ULL)
        return (seq_t)1;

    /* Working variables for the two binomial sequences. */
    seq_t A = 1;          /* C(3*n + 0 - 1, 0) = 1 */
    seq_t B = 0;          /* Will start from C(n-1,0) = 1 at k = 1 */
    seq_t sum = 0;

    /* Loop over k = 1 .. n.  For each k we update A and B as follows:
       A_k   = C(3*n + k - 1, k)
       B_k   = C(n-1, k-1)   (using complement property)    */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update A:  A_k = A_{k-1} * (3*n + k - 1) / k    */
        A = (A * (seq_t)(3ULL * n + k - 1)) / (seq_t)k;

        /* Update B:
           B_1 = C(n-1,0) = 1
           For k >= 2:
           B_k = B_{k-1} * (n - k + 1) / (k - 1)          */
        if (k == 1)
            B = (seq_t)1;
        else
            B = (B * (seq_t)(n - k + 1)) / (seq_t)(k - 1);

        /* Add contribution to the sum. */
        sum += A * B;
    }

    return sum;
}
