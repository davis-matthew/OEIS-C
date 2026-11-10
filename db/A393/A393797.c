/*
 * A393797  Position where n first occurs in A393336.
 * https://oeis.org/A393797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m such that A393336(m) = n (m has at most 2 zero
 *            bits); found by search over bit-length k: A393336 = k(k+1)/2 -
 *            2*(sum of zero positions)
 * Range:     u8 n <= 22, u16 n <= 106, u32 n <= 466, u64 n <= 1954,
 *            u128 n <= 8002, big any n
 * Validated: standard, pass, 71 terms through n = 9648 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A393797_p2(idx_t e)
{
    seq_t r = 1;
    while (e--) r = r * 2;
    return r;
}

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; k++) {
        idx_t T = k * (k + 1) / 2;
        if (T < n || ((T - n) & 1)) continue;
        idx_t S = (T - n) / 2;
        seq_t full = A393797_p2(k) - 1;
        if (S == 0) return full;
        int have = 0;
        seq_t best = 0;
        /* two zeros p<q, p+q=S, q<=k-1; smallest p gives largest q */
        {
            idx_t p = 1;
            if (S > k - 1) p = S - (k - 1);
            if (p < S - p) {
                seq_t r = A393797_p2(p - 1) + A393797_p2(S - p - 1);
                best = r; have = 1;
            }
        }
        /* one zero */
        if (S <= k - 1) {
            seq_t r = A393797_p2(S - 1);
            if (!have || r > best) { best = r; have = 1; }
        }
        if (have) return full - best;
    }
}
