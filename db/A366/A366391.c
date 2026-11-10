/*
 * A366391  Numbers k such that A163511(k) is an eleventh power.
 * https://oeis.org/A366391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k is a term iff, writing k's set bits at 0-indexed positions
 *            c_1<...<c_r, A163511(k) is an 11th power, i.e. c_i == i-1 (mod
 *            11) for i<r and c_r == r+9 (mod 11); a(1)=0, then the terms are
 *            unranked by DP counting
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A366391_G[66][11];
static int A366391_init = 0;

static void A366391_setup(void)
{
    int p, s;
    for (p = 0; p < 66; p++)
        for (s = 0; s < 11; s++)
            A366391_G[p][s] = 0;
    A366391_G[0][0] = 1;
    for (p = 0; p < 65; p++) {
        for (s = 0; s < 11; s++) {
            A366391_G[p + 1][s] += A366391_G[p][s];
        }
        {
            int sc = p % 11;
            A366391_G[p + 1][(sc + 1) % 11] += A366391_G[p][sc];
        }
    }
    A366391_init = 1;
}

seq_t a(idx_t n)
{
    int t, p, target;
    uint64_t m;
    seq_t k;
    if (!A366391_init) A366391_setup();
    if (n <= 1) return (seq_t)0;
    m = (uint64_t)(n - 2);
    for (t = 0; t < 64; t++) {
        uint64_t w = A366391_G[t][(t + 1) % 11];
        if (m < w) break;
        m -= w;
    }
    k = (seq_t)1 << t;
    target = (t + 1) % 11;
    for (p = t - 1; p >= 0; p--) {
        uint64_t zero_ways = A366391_G[p][target];
        if (m < zero_ways) continue;
        m -= zero_ways;
        k |= ((seq_t)1 << p);
        target = (target + 10) % 11;
    }
    return k;
}
