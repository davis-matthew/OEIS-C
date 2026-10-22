/*
 * A319656  Write n in 8-ary, sort digits into increasing order.
 * https://oeis.org/A319656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the integer whose base-8 representation consists of the
 *            digits of n written in base 8, sorted in nondecreasing order,
 *            interpreted back as a base‑8 number
 * Range:     u8 n <= 291, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned char cnt[8] = {0};
    idx_t tmp = n;
    /* Count the octal digits of n */
    while (tmp > 0) {
        unsigned d = tmp & 7;   /* tmp % 8 */
        cnt[d]++;
        tmp >>= 3;              /* tmp / 8 */
    }

    seq_t res = 0;
    for (int d = 0; d < 8; ++d) {
        unsigned c = cnt[d];
        while (c--) {
            res = res * 8 + d;
        }
    }
    return res;
}
