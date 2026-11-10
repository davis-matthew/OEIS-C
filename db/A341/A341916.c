/*
 * A341916  Inverse permutation to A341915.
 * https://oeis.org/A341916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = A006068(A059893(n)), a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 83 terms through n = 8160 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A341916_rev(idx_t n)
{
    if (n == 0) return (seq_t)0;
    unsigned m = 0;
    idx_t tmp = n;
    while ((tmp >>= 1) != 0) ++m;
    seq_t result = (seq_t)1 << m;
    for (unsigned i = 0; i < m; ++i) {
        if ((n >> i) & 1)
            result |= (seq_t)1 << (m - 1 - i);
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = A341916_rev(n);
    seq_t result = x;
    seq_t shifted = x;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}
