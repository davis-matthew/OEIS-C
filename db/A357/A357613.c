/*
 * A357613  Triangle read by rows T(n, k) = binomial(2*n, k) * binomial(3*n -
 *          k, 2*n).
 * https://oeis.org/A357613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(2*row, k) * binomial(3*row - k, 2*row) where
 *            (row, k) are obtained by flattening the triangle T(n,k) read by
 *            rows
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: binomial(idx_t n, idx_t k) from the helpers */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row number 'row' such that the total number of elements
       before that row is <= n, while the next row exceeds n. */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        row++;

    /* Number of elements before the current row */
    idx_t offset = row * (row + 1) / 2;

    /* Column index within the current row */
    idx_t k = n - offset;

    /* The required term is the product of two binomial coefficients */
    seq_t first  = binomial(2 * row, k);
    seq_t second = binomial(3 * row - k, 2 * row);

    return first * second;
}
