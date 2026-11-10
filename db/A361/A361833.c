/*
 * A361833  Fixed points of A361832.
 * https://oeis.org/A361833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th fixed point of A361832 (a(1)=0). A w-digit ternary
 *            fixed point has digits d_s, s=0..w-1, satisfying d_s = (-1)^s *
 *            Sum_{j<=s} C(s,j) d_j mod 3. Odd-position digits are determined
 *            by earlier ones. d_0 is 1 or 2, and even-position digits s>=2
 *            are free. This gives 2*3^floor((w-1)/2) term...
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    idx_t k = n - 2;   /* 0-based rank among positive terms */
    unsigned w = 1;
    for (;;) {
        unsigned m = (w - 1) / 2;
        idx_t cnt = 2;
        for (unsigned i = 0; i < m; ++i) cnt *= 3;
        if (k < cnt) break;
        k -= cnt;
        ++w;
    }
    unsigned m = (w - 1) / 2;

    int d[160];
    int free_d[100];
    idx_t r = k;
    for (unsigned i = m; i >= 1; --i) {
        free_d[i] = (int)(r % 3);
        r /= 3;
    }
    d[0] = 1 + (int)r;

    /* Pascal triangle mod 3 */
    static int C[160][160];
    for (unsigned s = 0; s < w; ++s) {
        C[s][0] = 1;
        C[s][s] = 1;
        for (unsigned j = 1; j < s; ++j)
            C[s][j] = (C[s - 1][j - 1] + C[s - 1][j]) % 3;
    }

    for (unsigned s = 1; s < w; ++s) {
        if (s & 1U) {
            int L = 0;
            for (unsigned j = 0; j < s; ++j) L += C[s][j] * d[j];
            d[s] = L % 3;
        } else {
            d[s] = free_d[s / 2];
        }
    }

    seq_t res = 0;
    for (unsigned s = 0; s < w; ++s) res = res * 3 + (seq_t)d[s];
    return res;
}
