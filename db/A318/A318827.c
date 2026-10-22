/*
 * A318827  a(n) = n - gcd(n - 1, phi(n)).
 * https://oeis.org/A318827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n-1, phi(n))
 * Range:     u8 n <= 257, u16 n <= 9978, u32 n <= 9978, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    seq_t phi_n  = phi(n);            /* φ(n) */
    seq_t n_minus1 = (seq_t)n - 1;    /* n-1 as seq_t */
    seq_t g = gcd(n_minus1, phi_n);   /* gcd(n-1, φ(n)) */
    return (seq_t)n - g;              /* a(n) = n - g */
}
