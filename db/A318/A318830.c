/*
 * A318830  a(n) = phi(n) - gcd(phi(n), n-1).
 * https://oeis.org/A318830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) - gcd(phi(n), n-1)
 * Range:     u8 n <= 287, u16 n <= 9159, u32 n <= 9159, u64 n <= 9159,
 *            u128 n <= 9159, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t phi_n   = phi(n);          /* φ(n) */
    seq_t n_minus1 = (seq_t)n;       /* convert n to seq_t first */
    n_minus1 -= 1;                   /* n-1 as seq_t */
    seq_t g = gcd(phi_n, n_minus1);  /* gcd(φ(n), n-1) */
    return phi_n - g;                /* a(n) */
}
