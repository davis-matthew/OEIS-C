/*
 * A245497  a(n) = phi(n)^2/2, where phi(n) = A000010(n), the Euler totient
 *          function.
 * https://oeis.org/A245497
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n)^2/2, where phi is Euler's totient function for n
 *            >= 3. For n < 3 the function returns 0
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* undefined for offset range */

    /* Compute φ(n) by trial division */
    uint64_t nn = (uint64_t)n;
    uint64_t phi = nn;

    for (uint64_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            phi = phi / p * (p - 1);
            while (nn % p == 0)
                nn /= p;
        }
    }
    if (nn > 1)   /* last prime factor > sqrt(original n) */
        phi = phi / nn * (nn - 1);

    seq_t phi_n = (seq_t)phi;
    return (phi_n * phi_n) / (seq_t)2;
}
