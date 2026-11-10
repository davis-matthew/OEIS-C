/*
 * A259046  Smallest m such that A259043(m) = n.
 * https://oeis.org/A259046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A259043(m) = n. Computed by DP using the
 *            reverse of A259043(10x+d) = A259043(x+d) + d. With h(k,lo) =
 *            least m >= lo with A259043(m) = k, each candidate is m = 10y -
 *            9d, where y >= d+1, y >= ceil((lo+9d)/10) and A259043(y) = k-d.
 *            (m = k is the single-digit case.)
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 76 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *H = mem_new(seq_t, (N + 1) * 11);
    unsigned char *E = mem_new(unsigned char, (N + 1) * 11);
    for (size_t k = 0; k <= N; k++) {
        /* lo = 1 first (no d=0 branch), then others */
        for (int pass = 0; pass < 11; pass++) {
            int lo = (pass == 0) ? 1 : (pass == 1 ? 0 : pass);
            bool have = false;
            seq_t best = 0;
            if (k < 10 && (int)k >= lo) {
                have = true;
                best = (seq_t)k;
            }
            for (int d = 0; d <= 9 && (size_t)d <= k; d++) {
                seq_t cand;
                if (d == 0) {
                    if (lo <= 1) continue;
                    if (!E[k * 11 + 1]) continue;
                    cand = H[k * 11 + 1] * 10;
                } else {
                    int l2 = (lo + 9 * d + 9) / 10;
                    if (l2 < d + 1) l2 = d + 1;
                    size_t idx = (k - (size_t)d) * 11 + (size_t)l2;
                    if (!E[idx]) continue;
                    cand = H[idx] * 10 - (seq_t)(9 * d);
                }
                if (!have || cand < best) {
                    have = true;
                    best = cand;
                }
            }
            H[k * 11 + (size_t)lo] = best;
            E[k * 11 + (size_t)lo] = have ? 1 : 0;
        }
    }
    seq_t r = H[N * 11 + 0];
    mem_free(H);
    mem_free(E);
    return r;
}
