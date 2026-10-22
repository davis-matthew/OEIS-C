/*
 * A331173  a(n) = min(n, A263273(n)), where A263273 is bijective base-3
 *          reverse.
 * https://oeis.org/A331173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min(n, A263273(n))
 * Range:     u8 n <= 254, u16 n <= 9638, u32 n <= 9638, u64 n <= 9638,
 *            u128 n <= 9638, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t orig = n;
    if (n == 0)
        return 0;

    /* Maximum number of base‑3 digits for a 64‑bit integer is < 40; 64 is safe. */
    const unsigned int MAX_DIGITS = 64;
    unsigned int digits[MAX_DIGITS];
    idx_t len = 0;

    /* Convert n to base‑3 (digits[0] = LSB). */
    while (n > 0) {
        digits[len++] = (unsigned int)(n % 3);
        n /= 3;
    }

    /* Count trailing zeros in the base‑3 representation. */
    idx_t t = 0;
    while (t < len && digits[t] == 0)
        ++t;

    /* Integer obtained by reversing the base‑3 digits. */
    seq_t rev = 0;
    for (idx_t i = 0; i < len; ++i)
        rev = rev * 3 + (seq_t)digits[i];

    /* Multiply by 3^t.  t is small (<= 64), so a simple loop is fine. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < t; ++i)
        pow3 *= 3;

    seq_t a263273 = rev * pow3;

    seq_t sn = (seq_t)orig;
    return (a263273 < sn) ? a263273 : sn;
}
