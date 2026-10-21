/*
 * A264749  a(n) = floor(n/BL(n)) where BL(n) = A070939(n) is the binary
 *          length of n.
 * https://oeis.org/A264749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n / BL(n)), BL(n) = floor(log2(n)) + 1 for n>0,
 *            BL(0)=0
 * Range:     u8 n <= 273, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t bl = floor_log2_uint64(n) + 1;
    return ((seq_t)n) / (seq_t)bl;
}
