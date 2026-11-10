/*
 * A333693  Inverse permutation to A333692.
 * https://oeis.org/A333693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse of permutation A333692; a(2*n) = 2*a(n). Computed
 *            by undoing A333692's prefix reversals: scanning right to left,
 *            at each 1, reverse the bits to the left and including it
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 84 terms through n = 7652 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int bits[64];
    int L = 0;
    uint64_t x = (uint64_t)n;
    if (x == 0)
        return (seq_t)0;
    {
        uint64_t t = x;
        int len = 0;
        while (t) { len++; t >>= 1; }
        L = len;
        for (int i = 0; i < L; i++)
            bits[i] = (int)((x >> (L - 1 - i)) & 1);   /* bits[0] is MSB */
    }
    int p = L;   /* search indices strictly below p (0-based positions < p) */
    while (1)
    {
        int q = p - 1;
        while (q >= 0 && bits[q] == 0)
            q--;
        if (q < 0)
            break;
        /* reverse prefix [0..q] */
        for (int i = 0, j = q; i < j; i++, j--)
        {
            int tmp = bits[i];
            bits[i] = bits[j];
            bits[j] = tmp;
        }
        p = q;
        if (p == 0)
            break;
    }
    uint64_t r = 0;
    for (int i = 0; i < L; i++)
        r = (r << 1) | (uint64_t)bits[i];
    return (seq_t)r;
}
