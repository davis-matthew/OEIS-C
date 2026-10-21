/*
 * A261135  Decimal value a(n) of the binary number b(n) obtained by starting
 *          from n, sequentially concatenating all binary numbers down to 1
 *          and then sequentially concatenating all binary numbers from 2 up
 *          to n.
 * https://oeis.org/A261135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary_to_decimal( concat(
 *            binary(n),...,binary(1),binary(2),...,binary(n)) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t val = 0;
    /* descending part: n, n-1, ..., 1 */
    for (idx_t k = n; k > 0; --k) {
        idx_t len = floor_log2_uint64(k) + 1;   /* number of bits in k */
        for (idx_t i = 0; i < len; ++i) {
            seq_t bit = ((seq_t)(k >> (len - 1 - i))) & 1;
            val = (val << 1) | bit;
        }
    }
    /* ascending part: 2, 3, ..., n */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t len = floor_log2_uint64(k) + 1;
        for (idx_t i = 0; i < len; ++i) {
            seq_t bit = ((seq_t)(k >> (len - 1 - i))) & 1;
            val = (val << 1) | bit;
        }
    }
    return val;
}
