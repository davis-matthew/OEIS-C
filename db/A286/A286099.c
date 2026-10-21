/*
 * A286099  Square array read by antidiagonals: A(n,k) = T(n OR k, n AND k),
 *          where T(n,k) is sequence A001477 considered as a two-dimensional
 *          table, AND is bitwise-and (A004198) and OR is bitwise-or
 *          (A003986).
 * https://oeis.org/A286099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = Cantor(i & j, i | j), * where (i, j) are the
 *            antidiagonal coordinates of index n * given by
 *            antidiagonal_coords. The Cantor pairing function is * T(x, y) =
 *            (x + y)·(x + y + 1)/2 + y. Thus * * a(n) = ((i & j + i | j) * (i
 *            & j + i | j + 1)) / 2 + (i | j)
 * Range:     u8 n <= 252, u16 n <= 9519, u32 n <= 9519, u64 n <= 9519,
 *            u128 n <= 9519, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    seq_t and_val = (seq_t)(i & j);
    seq_t or_val  = (seq_t)(i | j);

    seq_t sum = and_val + or_val;
    seq_t prod = sum * (sum + (seq_t)1) / (seq_t)2;
    seq_t result = prod + or_val;

    return result;
}
