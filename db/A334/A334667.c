/*
 * A334667  For any number with binary expansion (b_1, ..., b_w), replace the
 *          i-th "1" by b_{w+1-i} for i = 1..A000120(n) and the j-th "0" by
 *          b_j for j = 1..A023416(n); the resulting binary expansion is that
 *          of a(n).
 * https://oeis.org/A334667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained from the binary expansion of n by * replacing
 *            each 1 with the bit mirrored from the opposite side * of the
 *            word, and each 0 with the bit at the same (original) * position.
 *            The resulting binary expansion (treated as a binary * number) is
 *            a(n)
 * Range:     u8 n <= 256, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute number of bits w (w ≥ 1, even if n==0) */
    size_t w = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++w;
        tmp >>= 1;
    }
    if (w == 0) w = 1;          /* n == 0 case */

    /* store original bits: bits[0] is the most significant bit */
    unsigned char bits[64];
    for (size_t i = 0; i < w; ++i) {
        bits[i] = (unsigned char)((n >> (w - 1 - i)) & 1U);
    }

    size_t ones  = 0;
    size_t zeros = 0;
    seq_t result = 0;

    for (size_t pos = 0; pos < w; ++pos) {
        unsigned char orig = bits[pos];
        unsigned char repl;

        if (orig) {                     /* original bit is 1 */
            repl = bits[w - (ones + 1)]; /* mirrored position */
            ++ones;
        } else {                        /* original bit is 0 */
            repl = bits[zeros];          /* same position as original */
            ++zeros;
        }

        result = result * 2 + repl;     /* append bit to binary number */
    }

    return result;
}
