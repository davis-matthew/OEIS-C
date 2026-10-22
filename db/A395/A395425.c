/*
 * A395425  In the binary expansion of n without leading zeros, say (b_1, b_2,
 *          ..., b_w), reverse even-indexed bits.
 * https://oeis.org/A395425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = reverse all even-indexed bits of n when written in binary
 *            without leading zeros
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* compute bit length w of n */
    idx_t w = 0;
    idx_t tmp = n;
    while (tmp) {
        ++w;
        tmp >>= 1;
    }

    /* number of even positions */
    idx_t e = w >> 1;               /* floor(w/2) */

    /* store the even bits from MSB to LSB, in original order */
    unsigned char evens[128];       /* 128 is enough for the widest seq_t (128 bits) */
    for (idx_t i = 0; i < e; ++i) {
        idx_t pos = 2 * (i + 1);           /* 1‑based even position */
        idx_t idx = w - pos;              /* corresponding LSB index (0‑based) */
        evens[i] = (unsigned char)((n >> idx) & 1ULL);
    }

    /* build the result bit by bit, reversing the even positions */
    seq_t res = 0;
    for (idx_t pos = 1; pos <= w; ++pos) {
        res <<= 1;
        unsigned char bit;
        if (pos & 1) {                     /* odd position – keep the bit */
            idx_t idx = w - pos;
            bit = (unsigned char)((n >> idx) & 1ULL);
        } else {                           /* even position – reversed */
            idx_t i = pos >> 1;             /* i = pos/2 */
            bit = evens[e - i];
        }
        res |= bit;
    }

    return res;
}
