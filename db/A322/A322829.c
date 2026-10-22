/*
 * A322829  a(n) = Jacobi (or Kronecker) symbol (n/21).
 * https://oeis.org/A322829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Jacobi symbol (n/21) Since 21 = 3·7 and the Jacobi symbol
 *            is multiplicative, a(n) = 0 if n shares a factor with 21;
 *            otherwise a(n) = (n/3)⋅(n/7). For odd primes p, the Legendre
 *            symbol (n/p) can be read from n mod p: (n/3) = 1 for n≡1 (mod
 *            3), –1 for n≡2 (mod 3); (n/7) = 1 for n≡1,2,4 (mod...
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Zero if n is not coprime with 21 */
    if ((n % 3u) == 0u || (n % 7u) == 0u)
        return (seq_t)0;

    /* Legendre symbol (n/3) */
    int s3 = ((n % 3u) == 1u) ? 1 : -1;

    /* Legendre symbol (n/7) */
    unsigned rem7 = (unsigned)(n % 7u);
    int s7;
    switch (rem7) {
        case 1u: case 2u: case 4u: s7 = 1; break;
        case 3u: case 5u: case 6u: s7 = -1; break;
        default:   /* unreachable, handled by the divisor test above */
            s7 = 0;
    }

    return (seq_t)(s3 * s7);
}
