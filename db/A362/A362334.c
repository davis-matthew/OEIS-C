/*
 * A362334  a(n) = A000010(n) + A000010(n+2), where A000010 is the Euler
 *          phi-function.
 * https://oeis.org/A362334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = phi(n) + phi(n+2)
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t phi_n  = calc_phi(m);
    seq_t phi_n2 = calc_phi(m + 2);
    return phi_n + phi_n2;
}
