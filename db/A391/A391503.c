/*
 * A391503  a(n) = Kronecker symbol (10/n).
 * https://oeis.org/A391503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (10/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 0 */
    if (n == 0)
        return 0;

    /* Special case n = 1: (10/1) = 1 by definition */
    if (n == 1)
        return 1;

    /* If n is even or divisible by 5 the Kronecker symbol is 0 */
    if ((n & 1) == 0)       /* even → 0 */
        return 0;
    if (n % 5 == 0)         /* n ≡ 0 (mod 5) → 0 */
        return 0;

    /* For odd n not divisible by 5 we can use the Jacobi symbol */
    return (seq_t)jacobi_symbol(10, n);
}
