/*
 * A244807  The hexagonal spiral of Champernowne, read along the East (or
 *          90-degree) ray.
 * https://oeis.org/A244807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit of the Champernowne constant at position
 *            3*n^2-8*n+6 (the East ray of the hexagonal Champernowne spiral)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   champernowne_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/champernowne_digit.h"

seq_t a(idx_t n)
{
    idx_t N = n;
    idx_t pos = 3 * N * N - 8 * N + 6;   /* position in Champernowne */
    return champernowne_digit(pos);
}
