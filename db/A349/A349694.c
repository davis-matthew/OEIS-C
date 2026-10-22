/*
 * A349694  Dirichlet convolution of the squarefree kernel function (A007947)
 *          with itself.
 * https://oeis.org/A349694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of (e-1)*p^2 +
 *            2*p
 * Range:     u8 n <= 41, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* multiplicative for n≥1, a(1) = 1 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Factor 2 separately */
    if ((m & 1ULL) == 0) {
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t term = ((seq_t)(e - 1)) * (seq_t)2 * (seq_t)2 + ((seq_t)2) * (seq_t)2;
        result *= term;
    }

    /* Odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = ((seq_t)(e - 1)) * (seq_t)p * (seq_t)p + ((seq_t)2) * (seq_t)p;
            result *= term;
        }
    }

    /* Remaining prime factor if any */
    if (m > 1) {
        seq_t term = (seq_t)2 * (seq_t)m;
        result *= term;
    }

    return result;
}
