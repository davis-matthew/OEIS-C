/*
 * A371976  For any nonnegative integer n with binary expansion Sum_{i >= 0}
 *          b_i * 2^i, the binary expansion of a(n) is Sum_{i >= 0} c_i * 2^i
 *          with c_i = (Sum_{k > 0} b_{k*(i+1)-1}) mod 2 for any i >= 0.
 * https://oeis.org/A371976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binary transform of n: cᵢ = (∑_{k>0} b_{k·(i+1)-1}) mod
 *            2, where bⱼ are bits of n
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t bitlen_idx(idx_t x)
{
    if (x == 0)
        return 1;
    idx_t len = 0;
    while (x)
    {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t bits   = bitlen_idx(n);
    idx_t wbits  = (idx_t)sizeof(seq_t) * 8;

    /* we cannot set bits beyond the width of seq_t */
    idx_t max_i = bits;
    if (max_i > wbits)
        max_i = wbits;

    seq_t result = 0;

    for (idx_t i = 0; i < max_i; ++i)
    {
        int parity = 0;
        idx_t step = i + 1;                 /* distance between relevant positions */
        for (idx_t pos = step - 1; pos < bits; pos += step)
            parity ^= (int)((n >> pos) & 1);

        if (parity)
            result |= ((seq_t)1 << i);
    }

    return result;
}
