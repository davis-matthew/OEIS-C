/*
 * A307651  a(n) is the determinant of the Vandermonde matrix of the digits of
 *          n.
 * https://oeis.org/A307651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = ∏_{0≤i<j<k} (d_j - d_i) where d_i are the decimal digits
 *            of n in order
 * Range:     i8 n <= 158, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 specially: its only digit is 0, so det = 1. */
    if (n == 0)
        return 1;

    /* Extract digits from least significant to most, then reverse into the correct order. */
    uint8_t rev[64];
    size_t revlen = 0;
    uint64_t t = (uint64_t)n;
    while (t > 0) {
        rev[revlen++] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* For safety, ensure at least one digit. (Impossible for n>0, but keep for consistency.) */
    if (revlen == 0) {
        revlen = 1;
        rev[0] = 0;
    }

    uint8_t digits[64];
    for (size_t i = 0; i < revlen; ++i)
        digits[i] = rev[revlen - 1 - i];

    /* Compute the Vandermonde determinant product. */
    seq_t prod = 1;
    for (size_t i = 0; i + 1 < revlen; ++i) {
        seq_t di = digits[i];
        for (size_t j = i + 1; j < revlen; ++j) {
            seq_t diff = (seq_t)digits[j] - di;
            prod *= diff;
        }
    }

    return prod;
}
