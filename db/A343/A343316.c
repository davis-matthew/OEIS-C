/*
 * A343316  Array T(n, k), n, k > 0, read by antidiagonals; the balanced
 *          ternary representation of T(n, k) is obtained by multiplying
 *          componentwise the digits in the balanced ternary representations
 *          of n and of k.
 * https://oeis.org/A343316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, base
 * Formula:   a(n) = T(i,j) where (i,j)=antidiagonal_coords(n); T(i,j) is
 *            defined by componentwise multiplication of the balanced‑ternary
 *            digits of i and j
 * Range:     i8 n <= 9772, i16 n <= 9772, i32 n <= 9772, i64 n <= 9772,
 *            i128 n <= 9772, big any n
 * Helpers:   antidiagonal_coords, balanced_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/balanced_product.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return balanced_product(i, j);
}
