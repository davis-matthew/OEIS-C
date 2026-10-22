/*
 * A384120  Array read by antidiagonals: T(n,m) is the number of cliques in
 *          the n X m rook graph K_n X K_m.
 * https://oeis.org/A384120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i, j), where (i, j) are the coordinates on the n‑th *
 *            antidiagonal of the array T defined by * * T(n, m) = 1 + n*(2^m
 *            - 1) + m*(2^n - 1) - n*m . * * The mapping from a linear index n
 *            to coordinates (i, j) is: * * let s = floor( (sqrt(8*n + 1) -
 *            1)/2 ) // sum i + j * let t = s*(s + 1)/2 /...
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute s = floor((sqrt(8*n + 1) - 1)/2) with a small guard to
     * avoid rounding errors from floating‑point sqrt.                                  */
    long double root = sqrtl(8.0L * n + 1.0L);
    idx_t s = (idx_t)((root - 1.0L) / 2.0L);

    /* Adjust s if floating rounding caused an off‑by‑one error.   */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t t = s * (s + 1) / 2;   /* first index of this diagonal */
    idx_t i = n - t;             /* horizontal coordinate   */
    idx_t j = s - i;             /* vertical coordinate     */

    /* Compute powers of two.  pow_uint returns a seq_t.  */
    seq_t pow2m = pow_uint(2, j);
    seq_t pow2n = pow_uint(2, i);

    /* T(i, j) = 1 + i*(2^j - 1) + j*(2^i - 1) - i*j  */
    seq_t result = 1
        + (seq_t)i * (pow2m - 1)
        + (seq_t)j * (pow2n - 1)
        - (seq_t)i * (seq_t)j;

    return result;
}
