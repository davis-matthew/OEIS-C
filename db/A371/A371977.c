/*
 * A371977  Inverse permutation to A371976.
 * https://oeis.org/A371977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse of A371976 (GF(2)-linear map): with 1-indexed
 *            bits c_m of n, bit m of a(n) is XOR of c_{m*s} over squarefree
 *            s, m*s <= bitlength(n) (Mobius inversion mod 2)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 8053 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A371977_sqfree(idx_t s)
{
    for (idx_t d = 2; d * d <= s; ++d)
        if (s % (d * d) == 0)
            return 0;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t L = 0;
    for (idx_t x = n; x; x >>= 1)
        L++;

    idx_t wbits = (idx_t)sizeof(seq_t) * 8;
    seq_t result = 0;

    for (idx_t m = 1; m <= L; ++m)
    {
        int parity = 0;
        for (idx_t s = 1; m * s <= L; ++s)
        {
            if (A371977_sqfree(s))
                parity ^= (int)((n >> (m * s - 1)) & 1);
        }
        if (parity && (m - 1) < wbits)
            result |= ((seq_t)1 << (m - 1));
    }
    return result;
}
