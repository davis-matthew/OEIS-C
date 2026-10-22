/*
 * A319725  Write n in 8-ary, sort digits into decreasing order.
 * https://oeis.org/A319725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = sort the base‑8 digits of n in decreasing order
 * Range:     u8 n <= 67, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count 0‑7 digits of n in base‑8. */
    uint8_t count[8] = {0};
    idx_t t = n;
    if (t == 0) {
        return 0;
    }
    while (t > 0) {
        uint8_t d = t % 8;
        count[d]++;
        t /= 8;
    }

    /* Build the result by appending digits from 7 down to 0. */
    seq_t res = 0;
    for (int d = 7; d >= 0; --d) {
        uint8_t c = count[d];
        for (uint8_t i = 0; i < c; ++i) {
            res = res * 8 + d;
        }
    }
    return res;
}
