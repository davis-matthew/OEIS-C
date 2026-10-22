/*
 * A323170  a(n) = 1 if (2*phi(n)) < n, 0 otherwise, where phi is Euler
 *          totient function (A000010).
 * https://oeis.org/A323170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2*φ(n) < n, 0 otherwise where φ is Euler's totient
 *            function. The function computes φ(n) by trial division, which is
 *            fast enough for the required n up to about 10 000. The
 *            comparison is performed in uint64_t to avoid overflow when seq_t
 *            is less than 64 bits
 * Range:     u8 n <= 9232, u16 n <= 9232, u32 n <= 9232, u64 n <= 9232,
 *            u128 n <= 9232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute φ(n) with uint64_t arithmetic. */
    uint64_t nn   = (uint64_t)n;     /* local copy of n */
    uint64_t phi  = nn;
    uint64_t p;

    for (p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            while (nn % p == 0)
                nn /= p;
            phi -= phi / p;
        }
    }
    if (nn > 1)
        phi -= phi / nn;

    /* 2*φ(n) < n */
    return ( (2 * phi < (uint64_t)n) ? (seq_t)1 : (seq_t)0 );
}
