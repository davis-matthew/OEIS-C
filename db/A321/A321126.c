/*
 * A321126  T(n,k) = max(n + k - 1, n + 1, k + 1), square array read by
 *          antidiagonals (n >= 0, k >= 0).
 * https://oeis.org/A321126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of n
 *            * (i.e., the pair (n,k) visited while reading the square array *
 *            T(n,k)=max(n+k-1,n+1,k+1) row‑by‑row by antidiagonals)
 * Range:     u8 n <= 9303, u16 n <= 9303, u32 n <= 9303, u64 n <= 9303,
 *            u128 n <= 9303, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Compute the three candidates for T(i,j) */
    seq_t term1;
    if (i + j == 0)
        term1 = (seq_t)0;          /* handle n=j=0 case */
    else
        term1 = (seq_t)(i + j - 1);

    seq_t term2 = (seq_t)(i + 1);
    seq_t term3 = (seq_t)(j + 1);

    /* Return the maximum of the three terms */
    seq_t res = term1;
    if (term2 > res) res = term2;
    if (term3 > res) res = term3;
    return res;
}
