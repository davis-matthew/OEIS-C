/*
 * A356546  Triangle read by rows. T(n, k) = RisingFactorial(n + 1, n) / (k! *
 *          (n - k)!).
 * https://oeis.org/A356546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2 *
 *            T(r,k) = binomial(2r,r) * binomial(r,k) * (triangle read by
 *            rows, see OEIS A356546)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* finds the row index r such that n lies in that row of the triangle */
static idx_t _triangle_row(idx_t n)
{
    /* r(r+1)/2 <= n < (r+1)(r+2)/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = _triangle_row(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    seq_t t1 = binomial(2 * r, r);  /* C(2r, r)  */
    seq_t t2 = binomial(r, k);      /* C(r, k)   */

    return t1 * t2;                  /* product fits in seq_t when desired */
}
