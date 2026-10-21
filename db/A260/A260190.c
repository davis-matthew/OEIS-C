/*
 * A260190  Kronecker symbol(-6 / 2*n + 1).
 * https://oeis.org/A260190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Kronecker(-6, 2*n + 1) (for n ≥ 0) Since 2*n+1 is an odd
 *            positive integer, the Kronecker symbol reduces to the Jacobi
 *            symbol. The only special case is m = 1, for which the Jacobi
 *            symbol is defined to be 1. Thus a(n) = (−1/m) · (6/m)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* m = 2*n + 1 is always odd and positive */
    uint64_t m = (uint64_t)2 * n + 1ULL;

    /* Special case: m == 1  →  Kronecker(-6,1) = 1 */
    if (m == 1ULL)
        return 1;

    /* Legendre symbol (−1/m) = (−1)^((m-1)/2) */
    int sign_minus_one = (m % 4 == 1) ? 1 : -1;

    /* Jacobi symbol (6/m)  –  provided by the utility header */
    int sign_six = jacobi_symbol(6, m);

    return (seq_t)(sign_minus_one * sign_six);
}
