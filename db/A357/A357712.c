/*
 * A357712  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. cosh( sqrt(k) * log(1-x) ).
 * https://oeis.org/A357712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357712: a(n) is read by anti‑diagonals of the square array
   T(n,k) defined by
       T(0,k) = 1,   T(1,k) = 0,
       T(n,k) = (2*n-3)*T(n-1,k) - (n*n-4*n+4-k)*T(n-2,k)   for n≥2. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2. */
    idx_t m = 0;
    /* use unsigned __int128 to avoid overflow in the product */
    while ((unsigned __int128)(m + 1) * (m + 2) / 2 <= n)
        ++m;

    /* offset of n within this diagonal */
    idx_t offset = n - (unsigned __int128)m * (m + 1) / 2;
    idx_t row = offset;         /* n in the recurrence */
    idx_t col = m - offset;     /* k in the recurrence */

    /* Base cases T(0,k)=1, T(1,k)=0. */
    if (row == 0)
        return (seq_t)1;
    if (row == 1)
        return (seq_t)0;

    /* Compute T(row,col) iteratively. */
    seq_t prev2 = (seq_t)1;   /* T(0,col) */
    seq_t prev1 = (seq_t)0;   /* T(1,col) */
    seq_t curr = prev1;

    for (idx_t r = 2; r <= row; ++r)
    {
        /* (2*r-3) is always positive for r≥2. */
        seq_t term1 = (seq_t)(2 * r - 3) * prev1;

        /* Compute the signed coefficient: col - r^2 + 4*r - 4. */
        int64_t coeff = (int64_t)col - (int64_t)r * (int64_t)r
                       + 4 * (int64_t)r - 4;

        /* Depending on the sign of coeff, add or subtract the second term. */
        if (coeff >= 0)
        {
            seq_t term2 = (seq_t)coeff * prev2;
            curr = term1 + term2;
        }
        else
        {
            seq_t term2 = (seq_t)(-coeff) * prev2;
            curr = term1 - term2;
        }

        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
