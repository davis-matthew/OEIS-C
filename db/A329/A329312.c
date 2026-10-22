/*
 * A329312  Length of the co-Lyndon factorization of the binary expansion of
 *          n.
 * https://oeis.org/A329312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   length of the co-Lyndon factorization of the binary expansion of
 *            n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   inv_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/inv_bit.h"

seq_t a(idx_t n)
{
    /* Compute the number of bits in the binary representation of n */
    idx_t len = 0, tmp = n;
    while (tmp) { len++; tmp >>= 1; }

    seq_t count = 0;
    idx_t i = 0;
    while (i < len) {
        idx_t j = i + 1;
        idx_t k = i;
        /* Duval's algorithm on the inverted binary string */
        while (j < len) {
            unsigned char bk = inv_bit(n, len, k);
            unsigned char bj = inv_bit(n, len, j);
            if (bk <= bj) {
                if (bk < bj) k = i;
                else ++k;
                ++j;
            } else break;
        }
        idx_t step = j - k;
        while (i <= k) {
            i += step;
            ++count;
        }
    }
    return count;
}
