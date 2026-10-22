/*
 * A344622  a(n) = n*(n+1)/2 - sigma(n) + d(n).
 * https://oeis.org/A344622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - sigma(n) + d(n)
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* triangular number n*(n+1)/2 */
    seq_t nn = (seq_t)n;
    seq_t tri = nn*(nn+1)/2;

    /* compute sigma(n) and d(n) by prime factorization */
    idx_t m = n;                /* copy of n to factor */
    seq_t sigma = 1;            /* product of (p^{e+1}-1)/(p-1) factors */
    seq_t divcnt = 1;           /* product of (e+1) for each prime factor */

    for (idx_t p = 2; p*p <= m; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            seq_t term = 1;      /* p^0 */

            /* count exponent of prime p */
            while (m % p == 0) {
                m /= p;
                ++exp;
                term *= (seq_t)p;   /* now term == p^exp */
            }

            /* compute sigma factor: 1 + p + p^2 + ... + p^exp */
            seq_t sigma_factor = 1;
            seq_t pow = 1;
            for (unsigned i = 0; i < exp; ++i) {
                pow *= (seq_t)p;
                sigma_factor += pow;
            }

            sigma *= sigma_factor;
            divcnt *= (exp + 1);
        }
    }

    if (m > 1) {               /* remaining prime factor */
        sigma *= ((seq_t)m + 1);   /* (p^1 + 1) = 1 + p */
        divcnt *= 2;
    }

    /* a(n) = tri - sigma + d(n) */
    seq_t result = tri - sigma + divcnt;
    return result;
}
