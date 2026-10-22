/*
 * A362089  The base-3 expansion of a(n) is obtained by inserting a zero
 *          before each nonzero digit of the base-3 expansion of n.
 * https://oeis.org/A362089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by inserting a zero before each non‑zero digit
 *            of n's base‑3 representation
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    uint8_t digits[70];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0u) {
        digits[len++] = tmp % 3u;
        tmp /= 3u;
    }

    seq_t out = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        out *= (seq_t)3;
        if (d != 0u) {
            out *= (seq_t)3;
            out += (seq_t)d;
        }
    }
    return out;
}
