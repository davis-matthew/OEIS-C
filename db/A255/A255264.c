/*
 * A255264  Total number of ON cells in the "Ulam-Warburton" two-dimensional
 *          cellular automaton of A147562 after A048645(n) generations.
 * https://oeis.org/A255264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = A147562(A048645(n)); with m = A048645(n), A147562(m) = 1
 *            + 4*(S-1)/3 for m>=1, where S = sum_{k=0}^{m-1} 3^popcount(k) =
 *            sum over set bits i of m of 3^(set bits of m above i) * 4^i
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A147562_a(idx_t m)
{
    if (m == 0) return (seq_t)0;
    seq_t p4[64];
    p4[0] = (seq_t)1;
    for (int i = 1; i < 64; ++i) p4[i] = p4[i - 1] * (seq_t)4;
    seq_t S = 0;
    seq_t p3 = 1;
    for (int i = 63; i >= 0; --i) {
        if ((m >> i) & 1) {
            S += p3 * p4[i];
            p3 *= (seq_t)3;
        }
    }
    return (seq_t)1 + (seq_t)4 * ((S - (seq_t)1) / (seq_t)3);
}

seq_t a(idx_t n)
{
    idx_t h = 0;
    for (;;) {
        idx_t cum = (h + 1) * (h + 2) / 2;
        if (cum >= n) break;
        ++h;
    }
    idx_t cum_prev = h * (h + 1) / 2;
    idx_t pos = n - cum_prev;
    idx_t m = ((idx_t)1 << h);
    if (pos > 1) m += ((idx_t)1 << (pos - 2));
    return A147562_a(m);
}
