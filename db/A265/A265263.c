/*
 * A265263  Change every other 1 bit in binary expansion of n to 0.
 * https://oeis.org/A265263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = delete every other 1 bit in the binary representation of
 *            n, keeping the first 1, changing the second, keeping the third,
 *            etc
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>


seq_t a(idx_t n)
{
    seq_t res = 0;
    bool keep = true;
    const int bits = (int)(sizeof(idx_t) * CHAR_BIT);
    for (int i = bits - 1; i >= 0; --i) {
        if ((n >> i) & (idx_t)1) {
            if (keep) {
                res |= (seq_t)1 << i;
            }
            keep = !keep;
        }
    }
    return res;
}
