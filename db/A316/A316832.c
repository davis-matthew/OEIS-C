/*
 * A316832  In A316831, replace 2's and 3's with 0's.
 * https://oeis.org/A316832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n-th symbol of the fixed point of 0->01, 1->21,
 *            2->13, 3->33 is 1, else 0 (A316831 with 2's and 3's replaced by
 *            0's). Computed by walking the binary digits of n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Validated: partial, pass, 104 terms through n = 103 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char child[4][2] = {
        {0, 1},
        {2, 1},
        {1, 3},
        {3, 3}
    };
    if (n == 0)
        return (seq_t)0;

    int msb = 0;
    for (int i = 63; i > 0; --i) {
        if ((n >> i) & 1ULL) {
            msb = i;
            break;
        }
    }

    unsigned char s = 0;
    for (int i = msb; i >= 0; --i) {
        unsigned char bit = (unsigned char)((n >> i) & 1ULL);
        s = child[s][bit];
    }

    return (seq_t)(s == 1 ? 1 : 0);
}
