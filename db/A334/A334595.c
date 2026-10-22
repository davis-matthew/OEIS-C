/*
 * A334595  Binary interpretation of the right diagonal of the XOR-triangle
 *          with first row generated from the binary expansion of n.
 * https://oeis.org/A334595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = binary interpretation of the right diagonal of the
 *            XOR‑triangle whose first row is the binary expansion of n
 * Range:     u8 n <= 256, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    /* Compute the bit length of n. */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        L++;
        tmp >>= 1;
    }

    /* cur holds the current row of the XOR‑triangle.  */
    unsigned char cur[64];      /* 64 is enough for idx_t == uint64_t */
    for (idx_t i = 0; i < L; ++i) {
        /* bit position from msb to lsb */
        idx_t pos = L - 1 - i;
        cur[i] = (n >> pos) & 1;
    }

    seq_t ans = 0;
    for (idx_t len = L; len > 0; --len) {
        unsigned char right_bit = cur[len - 1];
        ans = (ans << 1) | right_bit;

        if (len == 1) break;          /* last row, nothing to generate */
        for (idx_t j = 0; j < len - 1; ++j)
            cur[j] ^= cur[j + 1];
    }
    return ans;
}
