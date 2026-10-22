/*
 * A304109  a(n) = 1 if binary expansion of n encodes a squarefree
 *          (0,1)-polynomial when the factorization is done in polynomial ring
 *          GF(2)[X], 0 otherwise.
 * https://oeis.org/A304109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the binary expansion of n encodes a squarefree
 *            (0,1)-polynomial over GF(2)[X], 0 otherwise. Implementation uses
 *            the squarefree test: gcd(f, f') = 1, where f' is the formal
 *            derivative
 * Range:     u8 n <= 9845, u16 n <= 9845, u32 n <= 9845, u64 n <= 9845,
 *            u128 n <= 9845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum value of n we expect is far below 2^64, so polynomial
   arithmetic fits into 64‑bit unsigned integers.  We perform the
   computation entirely on unsigned long long and return the result
   cast to seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence offset starts at 1 */
        return (seq_t)0;

    /* Polynomial f(x) represented by bits of n (LSB -> constant term). */
    unsigned long long f = (unsigned long long)n;

    /* f' = (f >> 1) & 0x5555... (masking bits that came from odd exponents). */
    const unsigned long long odd_mask = 0x5555555555555555ULL;
    unsigned long long der = (f >> 1) & odd_mask;   /* derivative */

    /* Euclidean algorithm for polynomials over GF(2). */
    unsigned long long a_poly = f;
    unsigned long long b_poly = der;

    while (b_poly != 0) {
        unsigned long long temp = a_poly;
        unsigned long long deg_b = 63 - __builtin_clzll(b_poly);

        while (temp != 0) {
            unsigned long long deg_t = 63 - __builtin_clzll(temp);
            if (deg_t < deg_b)
                break;
            temp ^= b_poly << (deg_t - deg_b);
        }
        a_poly = b_poly;
        b_poly = temp;
    }

    /* Squarefree iff gcd(f, f') == 1. */
    return (seq_t)(a_poly == 1 ? 1 : 0);
}
