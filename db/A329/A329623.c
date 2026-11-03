/*
 * A329623  The absolute value of the difference between n and A053392(n), the
 *          concatenation of the sums of every pair of consecutive digits of
 *          n.
 * https://oeis.org/A329623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = | n - A053392(n) | where A053392(n) is the concatenation
 *            of sums of every pair of consecutive digits of n
 * Range:     u8 n <= 136, u16 n <= 1645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9886 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline seq_t _A053392(idx_t n)
{
    /* Extract digits of n (least significant first) */
    unsigned char digits[20];
    size_t d = 0;
    if (n == 0) {
        digits[d++] = 0;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[d++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    /* If only one digit, the concatenation is 0 */
    if (d < 2) return (seq_t)0;

    seq_t result = (seq_t)0;
    /* Process pairs from most significant to least significant */
    for (size_t i = d - 1; i > 0; --i) {
        unsigned sum = digits[i] + digits[i - 1];
        if (sum >= 10) {
            result = result * (seq_t)100 + (seq_t)sum;
        } else {
            result = result * (seq_t)10 + (seq_t)sum;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    seq_t con = _A053392(n);
    return (val >= con) ? (val - con) : (con - val);
}
