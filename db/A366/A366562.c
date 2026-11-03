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
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the totient of n and the number of distinct prime factors.
   The function is written to work with any seq_t that supports
   normal arithmetic and signed values. */
static void phi_and_omega(idx_t n, seq_t *phi_out, int *omega_out)
{
    idx_t tmp = n;
    seq_t phi = n;
    int omega = 0;

    /* handle factor 2 */
    if ((tmp & 1ULL) == 0) {
        omega++;
        phi -= phi / 2;
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;
    }

    /* odd factors up to sqrt(tmp) */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            omega++;
            phi -= phi / p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        omega++;
        phi -= phi / tmp;
    }

    *phi_out   = phi;
    *omega_out = omega;
}

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
