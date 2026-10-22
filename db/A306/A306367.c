/*
 * A306367  a(n) = numerator of (n^2 + 2)/(n + 2).
 * https://oeis.org/A306367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, look
 * Formula:   a(n) = (n^2 + 2) / gcd(n + 2, 6)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the reduced numerator of the fraction (n^2+2)/(n+2). */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* cast index to seq_t type */
    seq_t num = nn * nn + 2;      /* n^2 + 2 */
    seq_t d   = gcd(nn + 2, (seq_t)6); /* gcd(n+2, 6) */
    return num / d;
}
