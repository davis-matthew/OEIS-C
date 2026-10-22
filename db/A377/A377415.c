/*
 * A377415  a(n) = n - A377414(n).
 * https://oeis.org/A377415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number obtained from the binary expansion of n by
 *            zeroing all bits at odd positions counting from the most
 *            significant bit. That is, if n = b1 b2 ... bk (b1 = 1), then the
 *            binary expansion of a(n) is c1 ... ck where ci = bi when i is
 *            even, otherwise ci = 0. (This matches the OEIS defi...
 * Range:     u8 n <= 767, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute bit length of n */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }

    seq_t res = 0;
    size_t bitsize = sizeof(seq_t) * 8;

    for (idx_t i = 2; i <= k; i += 2) {
        idx_t pos = k - i;                     /* position from LSB (0‑based) */
        if (pos >= bitsize)                    /* would overflow seq_t */
            continue;
        if ((n >> pos) & 1ULL)
            res |= ((seq_t)1 << pos);
    }

    return res;
}
