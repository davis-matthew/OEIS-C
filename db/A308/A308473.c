/*
 * A308473  Sum of numbers < n which have common prime factors with n.
 * https://oeis.org/A308473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n = 1, * otherwise a(n) = n * (n - phi(n) - 1) / 2 *
 *            where phi(n) is Euler's totient function
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 9849, u64 n <= 9849,
 *            u128 n <= 9849, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t phi_n = calc_phi(nn);
    return nn * (nn - phi_n - 1) / 2;
}
