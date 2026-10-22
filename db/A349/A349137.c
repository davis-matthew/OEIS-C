/*
 * A349137  a(n) = phi(A003602(n)), where A003602 is Kimberling's paraphrase
 *          of the binary number system, and phi is Euler totient function.
 * https://oeis.org/A349137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ((n+1)/2) when n is odd; a(n) = a(n/2) when n is even.
 *            This follows from the recurrence a(2k-1) = A000010(k) and a(2k)
 *            = a(k)
 * Range:     u8 n <= 512, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Reduce n to an odd integer by repeatedly dividing by 2.  */
    while ((n & 1) == 0) {
        n >>= 1;
    }
    /* For an odd index n = 2k-1, the value is φ(k).  */
    idx_t k = (n + 1) >> 1;
    return phi(k);
}
