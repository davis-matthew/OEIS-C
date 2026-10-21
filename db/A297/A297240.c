/*
 * A297240  Down-variation of the base-14 digits of n; see Comments.
 * https://oeis.org/A297240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the down-variation of n in base 14: sum of all *
 *            positive differences between adjacent base‑14 digits (higher
 *            minus lower). * The implementation simply scans the digits of n
 *            in base 14
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    if (temp < 14) return 0;      /* single digit -> no variation */

    seq_t sum = 0;
    uint8_t prev = (uint8_t)(temp % 14);
    temp /= 14;

    while (temp > 0) {
        uint8_t next = (uint8_t)(temp % 14);
        if (next > prev) sum += (seq_t)(next - prev);
        prev = next;
        temp /= 14;
    }

    return sum;
}
