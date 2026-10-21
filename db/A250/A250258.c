/*
 * A250258  Least nonnegative integer whose decimal digits divide the plane
 *          into n regions (A250257 variant).
 * https://oeis.org/A250258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-2) + 8 (n >= 5), with a(1) = 1, a(2) = 0, a(3) =
 *            8, a(4) = 68 (equivalently: for n >= 4, a(n) has floor(n/2)
 *            digits: - if n is even: first digit 6, rest 8; - if n is odd :
 *            all digits 8)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)8;
    /* n >= 4 */
    idx_t digits = n / 2;              /* floor(n/2) */
    seq_t res = 0;
    for (idx_t i = 0; i < digits; ++i) {
        seq_t digit = ((n & 1ULL) == 0 && i == 0) ? (seq_t)6 : (seq_t)8;
        res = res * (seq_t)10 + digit;
    }
    return res;
}
