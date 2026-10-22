/*
 * A366562  a(n) = Sum_{k=1..n} A366561(n,k)*A023900(k)/n.
 * https://oeis.org/A366562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [Mod[n,2] = 1] * phi(n) * (-1)^omega(n) where phi(n) is
 *            Euler's totient and omega(n) is the number of distinct prime
 *            factors of n. For even n the value is 0
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   phi_and_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_and_omega.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; for even n return 0. */
    if ((n & 1ULL) == 0)
        return 0;

    seq_t phi;
    int   omega;

    phi_and_omega(n, &phi, &omega);

    /* Multiply by (-1)^omega */
    if (omega & 1)
        return -phi;
    else
        return phi;
}
