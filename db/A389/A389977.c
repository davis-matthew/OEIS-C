/*
 * A389977  Absolute value of the inverse Möbius transform of
 *          n*mu(n)*sigma(n).
 * https://oeis.org/A389977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   multiplicative with a(p^e) = p^2 + p - 1; a(n) = product_{p|n}
 *            (p^2 + p - 1) (depends only on the distinct primes of n)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not defined for 0, but harmless */

    uint64_t nn = (uint64_t)n;           /* work in unsigned 64‑bit for factorization */
    seq_t res = 1;                       /* multiplicative identity */

    for (uint64_t p = 2; p <= nn / p; ++p)
    {
        if (nn % p == 0)
        {
            seq_t val = (seq_t)p * (seq_t)p + (seq_t)p - (seq_t)1;
            res *= val;
            while (nn % p == 0) nn /= p;    /* remove all powers of p */
        }
    }
    if (nn > 1)                          /* remaining prime factor > sqrt(original n) */
    {
        seq_t val = (seq_t)nn * (seq_t)nn + (seq_t)nn - (seq_t)1;
        res *= val;
    }
    return res;
}
