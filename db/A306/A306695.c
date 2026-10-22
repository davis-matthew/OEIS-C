/*
 * A306695  a(n) = gcd(n, psi(n)).
 * https://oeis.org/A306695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, ψ(n)) where ψ(n) = n * Π_{p|n}(1+1/p)
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, just to silence unused warning */

    seq_t n_t = (seq_t)n;
    seq_t psi = n_t;
    idx_t  temp = n;

    /* handle 2 separately */
    if (temp % 2 == 0) {
        psi = psi / 2 * (2 + 1);
        while (temp % 2 == 0) temp /= 2;
    }

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            psi = psi / p * (p + 1);
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        psi = psi / temp * (temp + 1);
    }

    return gcd(psi, n_t);
}
