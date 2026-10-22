/*
 * A362353  Triangle read by rows: T(n,k) = (-1)^(n-k)*binomial(n, k)*(k+3)^n,
 *          for n >= 0, and k = 0,1, ..., n. Coefficients of certain Sidi
 *          polynomials.
 * https://oeis.org/A362353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, look
 * Formula:   a(n) = T(row, col) where T(row, col) = (-1)^(row-col) ·
 *            binomial(row, col) · (col+3)^row and the triangle is read by
 *            rows, so that row = floor((sqrt(8·n+1)-1)/2) and col = n -
 *            row·(row+1)/2
 * Range:     i8 n <= 6, i16 n <= 17, i32 n <= 48, i64 n <= 127, i128 n <= 340,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row index of the flattening of the triangle. */
    idx_t row = 0;
    while (row * (row + 1) / 2 <= n) {
        row++;
    }
    row--;  /* row*(row+1)/2 <= n < (row+1)(row+2)/2 */

    /* Column index within the row. */
    idx_t prev = row * (row + 1) / 2;
    idx_t col  = n - prev;

    /* Compute the binomial coefficient and the power. */
    seq_t bin      = binomial(row, col);
    seq_t base     = (seq_t)(col + 3);
    seq_t power    = big_pow(base, (seq_t)row);

    /* Multiply and apply the sign. */
    seq_t result = bin * power;
    if ((row - col) & 1) {
        result = -result;
    }
    return result;
}
