/*
 * A338827  For any number with decimal representation (d(1), d(2), ...,
 *          d(k)), the decimal representation of a(n) is (abs(d(1)-d(k)),
 *          abs(d(2)-d(k-1)), ..., abs(d(k)-d(1))).
 * https://oeis.org/A338827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = decimal number formed from the absolute differences of
 *            mirrored decimal digits of n. For n with digits d₁…d_k, a(n) has
 *            digits |dᵢ−dₖ₊₁₋ᵢ| for i=1…k (leading zeros dropped)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert n to decimal digits, stored reversed in buf */
    unsigned char buf[25];
    idx_t len = 0;
    idx_t t = n;
    do {
        buf[len++] = t % 10;
        t /= 10;
    } while (t > 0);

    seq_t res = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned diff = buf[i] > buf[len - 1 - i] ? buf[i] - buf[len - 1 - i]
                                                 : buf[len - 1 - i] - buf[i];
        res = res * 10 + (seq_t)diff;
    }
    return res;
}
