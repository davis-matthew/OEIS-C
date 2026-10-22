/*
 * A369042  LCM-transform of the inverse of binary Gray code (A006068).
 * https://oeis.org/A369042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A006068(n)) A006068(n) is the inverse binary Gray
 *            code. A014963(m) gives the least prime p such that m is a power
 *            of p; if m is not a prime power the value is 1
 * Range:     u8 n <= 258, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   inverse_gray
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/inverse_gray.h"

seq_t a(idx_t n) {
    /* compute inverse Gray code of n */
    idx_t m = inverse_gray(n);
    if (m == 0 || m == 1) return (seq_t)1;

    /* check whether m is a prime power */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t tmp = m;
            while (tmp % p == 0) tmp /= p;
            return (tmp == 1) ? (seq_t)p : (seq_t)1;
        }
    }
    /* m is prime */
    return (seq_t)m;
}
