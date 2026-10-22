/*
 * A393845  a(n) is the smallest positive multiple of n whose decimal
 *          representation contains every distinct digit of n-1.
 * https://oeis.org/A393845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive multiple of n whose decimal
 *            representation contains every distinct digit of n-1
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digits_mask_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_mask_seq.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    unsigned target = digits_mask_seq(nn - (seq_t)1);

    seq_t m = nn;
    for (;;) {
        unsigned mmask = digits_mask_seq(m);
        if ((mmask & target) == target)
            return m;
        m += nn;
    }
}
