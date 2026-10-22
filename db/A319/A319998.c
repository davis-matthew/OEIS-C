/*
 * A319998  a(n) = Sum_{d|n, d is even} mu(n/d)*d, where mu(n) is Moebius
 *          function A008683.
 * https://oeis.org/A319998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*phi(n/2) for even n, 0 for odd n (A319998)
 * Range:     u8 n <= 261, u16 n <= 9351, u32 n <= 9351, u64 n <= 9351,
 *            u128 n <= 9351, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Odd indices give 0 */
    if (n & 1U) {
        return (seq_t)0;
    }
    /* Even indices: a(2k) = 2*phi(k) */
    idx_t half = n >> 1;
    seq_t p = phi(half);
    return p + p;   /* double the totient */
}
