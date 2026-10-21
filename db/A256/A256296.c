/*
 * A256296  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 0 to the
 *          digits of n written in base 6, then convert back to base 10.
 * https://oeis.org/A256296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i>=0} ((digit_i(n,6)+1) mod 6) * 6^i, where digit_i
 *            is the i‑th digit of n in base 6
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base‑6 digits of n, least‑significant first */
    uint8_t digits[70];
    size_t   dcnt = 0;
    idx_t    tmp  = n;

    if (tmp == 0) {
        digits[dcnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[dcnt++] = (uint8_t)(tmp % 6);
            tmp /= 6;
        }
    }

    /* Reconstruct number from transformed digits */
    seq_t result = 0;
    for (size_t i = dcnt; i > 0; --i) {
        uint8_t d       = digits[i - 1];
        uint8_t d_next  = d + 1;
        if (d_next == 6) d_next = 0;
        result = result * (seq_t)6 + d_next;
    }
    return result;
}
