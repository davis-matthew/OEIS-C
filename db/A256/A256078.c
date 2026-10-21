/*
 * A256078  Write n in binary, exchange digits '0' <-> '1'.
 * https://oeis.org/A256078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number formed by the bits of (~n & ((1ULL << k) -
 *            1)) with leading zeros removed. k = floor(log2(n)) + 1 for n >
 *            0, k = 1 for n = 0. For n = 0, a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 (binary "0") */
    if (n == 0) return (seq_t)1;

    /* Determine number of bits of n (k) */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    seq_t result = 0;
    bool started = false;

    /* Process bits from most significant to least significant */
    idx_t i = bits;
    while (i--) {
        unsigned bit = (unsigned)((n >> i) & 1U);
        unsigned flipped = 1U - bit;  /* 0 becomes 1, 1 becomes 0 */

        if (!started) {
            if (flipped == 0)
                continue;        /* skip leading zero */
            started = true;
        }

        result = result * 10 + (seq_t)flipped;
    }

    /* If all flipped bits were zero (e.g., n = 2^k - 1), result remains 0 */
    return result;
}
