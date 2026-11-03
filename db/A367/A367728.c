/*
 * A367728  a(n) is the denominator of (R(n) - 1)/(n + 1), where R(n) is the
 *          digit reversal of n.
 * https://oeis.org/A367728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac, look
 * Formula:   a(n) = (n+1) / gcd(R(n)-1, n+1) where R(n) is the decimal digit
 *            reversal of n
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8983 (2026-11-01)
 * Helpers:   gcd, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* R(0) = 0, (0-1)/(0+1) = -1/1 → denominator = 1 */
        return (seq_t)1;
    }

    /* Denominator of the fraction before reduction. */
    seq_t denom = (seq_t)(n + 1);

    /* Reversed decimal representation of n. */
    seq_t revn = (seq_t)reverse_digits_idx(n);

    /* Numerator of (R(n)-1)/(n+1). */
    seq_t num = revn - (seq_t)1;

    /* Reduce the fraction by dividing by the GCD. */
    seq_t g = gcd(num, denom);

    return denom / g;
}
