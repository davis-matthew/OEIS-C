/*
 * A254379  Characteristic function of the even odious numbers (A128309).
 * https://oeis.org/A254379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 if n is even and popcount(n) is odd (i.e. n is an even
 *            odious number), 0 otherwise
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9926 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If n is odd it cannot be an even odious number. */
    if ((n & 1ULL) != 0)
        return (seq_t)0;

    /* Compute the parity of the binary Hamming weight of n.
       parity == true  <=>  popcount(n) is odd. */
    unsigned long long m = n;
    bool parity = false;
    while (m) {
        parity ^= (m & 1ULL);
        m >>= 1;
    }

    return parity ? (seq_t)1 : (seq_t)0;
}
