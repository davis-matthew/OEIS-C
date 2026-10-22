/*
 * A354033  a(n) = 1 if n > 1 and n is a power of a prime of the form 4m+3,
 *          otherwise 0.
 * https://oeis.org/A354033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n > 1 and n is a power of a prime of the form 4m+3,
 *            otherwise 0
 * Range:     u8 n <= 9667, u16 n <= 9667, u32 n <= 9667, u64 n <= 9667,
 *            u128 n <= 9667, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t m = (uint64_t)n;
    uint64_t p = 0;

    /* Find smallest prime factor p */
    if ((m & 1ULL) == 0) {
        p = 2;
    } else {
        /* trial division by odd numbers */
        for (uint64_t d = 3; d <= m / d; d += 2) {
            if (m % d == 0) {
                p = d;
                break;
            }
        }
    }

    /* If m itself is prime */
    if (p == 0)
        p = m;

    /* Check if m is a pure power of p */
    uint64_t temp = m;
    while (temp % p == 0)
        temp /= p;
    if (temp != 1)
        return 0;

    /* Return 1 iff p ≡ 3 (mod 4) */
    return ((p & 3ULL) == 3ULL) ? (seq_t)1 : (seq_t)0;
}
