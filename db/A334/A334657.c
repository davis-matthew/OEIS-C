/*
 * A334657  Dirichlet g.f.: 1 / zeta(s-2).
 * https://oeis.org/A334657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = μ(n) * n^2 (μ = Möbius function)
 * Range:     i8 n <= 12, i16 n <= 181, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    int mu = 1;  /* Möbius sign for square‑free numbers */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1)   /* not square‑free */
                    return (seq_t)0;
            }
            mu = -mu;           /* toggle sign for one distinct prime */
        }
    }
    if (temp > 1) {              /* remaining prime factor */
        mu = -mu;
    }

    seq_t sq = (seq_t)n * (seq_t)n;
    return (mu == -1) ? -sq : sq;
}
