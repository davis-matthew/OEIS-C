/*
 * A297033  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-5
 *          digits of n; see Comments.
 * https://oeis.org/A297033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = # of maximal monotone runs (increasing, decreasing or
 *            constant) of consecutive base‑5 digits of n of length ≥ 2; any n
 *            with a single base‑5 digit yields 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base5_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/base5_digits.h"

seq_t a(idx_t n)
{
    uint8_t digits[64];
    size_t len;
    base5_digits(n, digits, &len);

    if (len <= 1)         /* single base‑5 digit ⇒ no runs of length ≥ 2 */
        return 0;

    seq_t pieces = 0;
    int prev_sign = 0;
    int first = 1;

    for (size_t i = 0; i + 1 < len; ++i) {
        int sign;
        if (digits[i + 1] > digits[i])
            sign = 1;
        else if (digits[i + 1] < digits[i])
            sign = -1;
        else
            sign = 0;

        if (first) {
            prev_sign = sign;
            first = 0;
        } else if (sign != prev_sign) {
            ++pieces;          /* end of one maximal run, start of another */
            prev_sign = sign;
        }
    }
    /* Count the final maximal run */
    ++pieces;
    return pieces;
}
