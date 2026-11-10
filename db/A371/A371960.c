/*
 * A371960  Inverse permutation to A371959.
 * https://oeis.org/A371960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   inverse of A371959. With binary digits c_1..c_w of n (c_1=1),
 *            a(n) has digits b_1..b_w where b_1=1 and b_k = (c_k +
 *            Sum_{i=2..ceiling(k/2)} b_i*b_{k+1-i}) mod 2
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 7288 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    int w = 0;
    uint64_t tmp = (uint64_t)n;
    while (tmp) {
        ++w;
        tmp >>= 1;
    }

    int c[64];
    int b[64];
    for (int i = 0; i < w; ++i)
        c[i] = (int)(((uint64_t)n >> (w - 1 - i)) & 1u);

    uint64_t result = 0;
    for (int k = 1; k <= w; ++k) {
        int v = c[k - 1];
        int half = (k + 1) / 2;
        for (int i = 2; i <= half; ++i)
            v ^= (b[i - 1] & b[k - i]);
        b[k - 1] = v;
        result = (result << 1) | (uint64_t)v;
    }
    return (seq_t)result;
}
