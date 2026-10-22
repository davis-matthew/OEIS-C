/*
 * A354438  Square array A(n, k), n, k >= 0, read by antidiagonals; the
 *          factorial base expansion of A(n, k) is obtained by adding
 *          componentwise and reducing modulo their radix the digits of the
 *          factorial base expansions of n and k.
 * https://oeis.org/A354438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) is the square array A(n,k) read by antidiagonals, where
 *            A(n,k) is defined by adding the factorial‑base expansions of n
 *            and k component‑wise modulo their radix
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   antidiagonal_coords, factorial_base_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function computes the value of the sequence */
#include "../utils/antidiagonal_coords.h"
#include "../utils/factorial_base_add.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);          /* get coordinates (i,j) of n on the antidiagonals */
    return factorial_base_add(i, j);          /* compute A(i,j) */
}
