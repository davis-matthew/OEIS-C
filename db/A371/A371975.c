/*
 * A371975  Inverse permutation to A371974.
 * https://oeis.org/A371975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse of A371974 within each binary length class;
 *            solves the GF(2) linear system c_k = XOR_{i=1 mod (w+1-k)} b_i
 *            by Gaussian elimination, a(0)=0
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 83 terms through n = 7336 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    int w = 0;
    idx_t t = n;
    while (t) { w++; t >>= 1; }

    uint64_t row[64];
    unsigned char rhs[64];

    for (int k = 1; k <= w; ++k) {
        int d = w + 1 - k;
        uint64_t m = 0;
        for (int i = 1; i <= w; i += d)
            m ^= ((uint64_t)1 << (i - 1));
        row[k - 1] = m;
        rhs[k - 1] = (unsigned char)((n >> (w - k)) & 1);
    }

    for (int j = 0; j < w; ++j) {
        int p = -1;
        for (int r = j; r < w; ++r) {
            if ((row[r] >> j) & 1) { p = r; break; }
        }
        if (p < 0)
            continue;
        if (p != j) {
            uint64_t tm = row[p]; row[p] = row[j]; row[j] = tm;
            unsigned char tr = rhs[p]; rhs[p] = rhs[j]; rhs[j] = tr;
        }
        for (int r = 0; r < w; ++r) {
            if (r != j && ((row[r] >> j) & 1)) {
                row[r] ^= row[j];
                rhs[r] ^= rhs[j];
            }
        }
    }

    seq_t ans = 0;
    for (int j = 0; j < w; ++j)
        ans = (ans << 1) | (seq_t)rhs[j];
    return ans;
}
