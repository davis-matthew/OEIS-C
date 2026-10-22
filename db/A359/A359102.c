/*
 * A359102  a(n) = phi(7 * n)/6.
 * https://oeis.org/A359102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(7 * n)/6
 * Range:     u8 n <= 36, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 7;          /* compute 7*n safely in seq_t */
    seq_t phi_val = calc_phi(m);     /* Euler's totient of 7*n */
    return phi_val / 6;              /* exact division by 6 */
}
