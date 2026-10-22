/*
 * A362414  a(n) = gcd(n, phi(n)^2) / gcd(n, phi(n)).
 * https://oeis.org/A362414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n,phi(n)^2)/gcd(n,phi(n))
 * Range:     u8 n <= 1781, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Convert the index to the generic sequence type */
    seq_t n_seq = (seq_t)n;

    /* Euler's totient of n */
    seq_t phi_n = phi(n);

    /* First gcd */
    seq_t g = gcd(n_seq, phi_n);

    /* n divided by that gcd */
    seq_t n_div_g = n_seq / g;

    /* a(n) = gcd(n/g, g) */
    seq_t a_val = gcd(n_div_g, g);

    return a_val;
}
