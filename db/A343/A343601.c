/*
 * A343601  For any positive number n, the ternary representation of a(n) is
 *          obtained by right-rotating the ternary representation of n until a
 *          nonzero digit appears again as the leftmost digit; a(0) = 0.
 * https://oeis.org/A343601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by right‑rotating the ternary representation of
 *            n until the leftmost digit is nonzero
 * Range:     u8 n <= 243, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
/* Computes OEIS A343601: a(0)=0; for n>0, rotate the base‑3 digits of n
   to the right until a non‑zero digit becomes the most significant digit. */
{
    if (n == 0)
        return 0;

    /* Buffer large enough for ternary digits of a 64‑bit integer (max 41 digits). */
    uint8_t buf[64];
    int len = 0;
    idx_t tmp = n;

    /* Convert to base 3, least significant digit first. */
    while (tmp > 0) {
        buf[len++] = tmp % 3;
        tmp /= 3;
    }

    /* Reverse to most significant digit first. */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t   = buf[i];
        buf[i]      = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }

    /* Locate the last (rightmost) non‑zero digit. */
    int k = len - 1;
    while (k >= 0 && buf[k] == 0)
        --k;

    /* Build the resulting number from the rotated digit order. */
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = val * 3 + (seq_t)buf[(k + i) % len];
    }
    return val;
}
