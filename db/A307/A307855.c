/*
 * A307855  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 1/sqrt(1 - 2*x + (1-4*k)*x^2).
 * https://oeis.org/A307855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 86, u32 n <= 223, u64 n <= 585,
 *            u128 n <= 1640, big any n
 * Helpers:   A_row_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A_row_k.h"
/* A307855: a(n) is the entry A(n,k) of the square array read by antidiagonals.
   A(n,k) is the coefficient of x^n in (1 + x + k*x^2)^n.
   Recurrence:
        n*A(n,k) = (2n-1)*A(n-1,k) - (1-4k)*(n-1)*A(n-2,k)
   Initial values:  A(0,k)=1,  A(1,k)=1. */

seq_t a(idx_t n)
{
    /* Locate the antidiagonal m that contains index n.
       The count of terms up to diagonal m-1 is m*(m+1)/2. */
    idx_t m = 0;
    idx_t terms_before = 0;
    while (terms_before + (m + 1) <= n)
    {
        terms_before += m + 1;
        ++m;
    }

    /* Coordinates within the diagonal: row = n - terms_before,
       column k = m - row. */
    idx_t row = n - terms_before;
    idx_t k   = m - row;

    return A_row_k(row, k);
}
