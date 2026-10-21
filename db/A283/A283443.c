/*
 * A283443  a(n) = lcm(n,6) / gcd(n,6).
 * https://oeis.org/A283443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,6) / gcd(n,6) Equivalent piecewise: n mod 6 = 0 ->
 *            a(n) = n/6 n mod 6 = 1 or 5 -> a(n) = 6*n n mod 6 = 2 or 4 ->
 *            a(n) = 3*n/2 n mod 6 = 3 -> a(n) = 2*n/3
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute gcd with 6 by examining n modulo 6 */
    unsigned char r = (unsigned char)(n % 6);
    seq_t g;
    switch (r) {
        case 0: g = (seq_t)6; break;
        case 1: case 5: g = (seq_t)1; break;
        case 2: case 4: g = (seq_t)2; break;
        case 3: g = (seq_t)3; break;
        default: g = (seq_t)1; /* never reached */
    }

    seq_t n_val = (seq_t)n;
    /* a(n) = 6 * n / g^2 */
    seq_t g2 = g * g;
    seq_t result = ((seq_t)6 * n_val) / g2;
    return result;
}
