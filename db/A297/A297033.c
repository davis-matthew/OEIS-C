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
 * Validated: standard, pass, 102 terms through n = 8667 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static void base5_digits(idx_t n, uint8_t *buf, size_t *len)
{
    /* Convert n to base‑5 digits in normal order (most‑significant first) */
    if (n == 0) {          /* n will never be 0 for this sequence, but keep safety */
        *len = 1;
        buf[0] = 0;
        return;
    }
    uint8_t tmp[64];            /* enough for uint64_t */
    size_t p = 0;
    while (n > 0) {
        tmp[p++] = n % 5;
        n /= 5;
    }
    *len = p;
    for (size_t i = 0; i < p; ++i)
        buf[i] = tmp[p - 1 - i];
}

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
