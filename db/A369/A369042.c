/*
 * A369042  LCM-transform of the inverse of binary Gray code (A006068).
 * https://oeis.org/A369042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A006068(n)) A006068(n) is the inverse binary Gray
 *            code. A014963(m) gives the least prime p such that m is a power
 *            of p; if m is not a prime power the value is 1
 * Range:     u8 n <= 258, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t inverse_gray(idx_t g) {
    idx_t inv = 0;
    for (idx_t t = g; t; t >>= 1) {
        inv ^= t;
    }
    return inv;
}

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
