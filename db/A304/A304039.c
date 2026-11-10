/*
 * A304039  Least number k such that A304036(k) = n.
 * https://oeis.org/A304039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A304036(k) = n, found by a DP over
 *            factorial-base digits of k with a 2-state transfer vector
 *            (search for the minimum)
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 112, u64 n <= 163,
 *            u128 n <= 163, big any n
 * Validated: standard, pass, 58 terms through n = 143 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    size_t N = (size_t)n;
    size_t W = N + 2;
    size_t total = W * W;
    seq_t *cur = mem_new(seq_t, total);
    seq_t *nxt = mem_new(seq_t, total);
    unsigned char *hc = mem_new(unsigned char, total);
    unsigned char *hn = mem_new(unsigned char, total);
    if (!cur || !nxt || !hc || !hn) {
        if (cur) mem_free(cur);
        if (nxt) mem_free(nxt);
        if (hc) mem_free(hc);
        if (hn) mem_free(hn);
        return 0;
    }
    for (size_t t = 0; t < total; ++t) { hc[t] = 0; hn[t] = 0; cur[t] = 0; nxt[t] = 0; }
    /* level 0: X_0 = (1, 0), k = 0 */
    hc[1 * W + 0] = 1;
    cur[1 * W + 0] = 0;

    bool have_best = false;
    seq_t best = 0;
    seq_t fac = 1;

    for (long long i = 1; i <= 45; ++i) {
        fac = fac * (seq_t)i;
        if (have_best && fac > best) break;
        for (size_t t = 0; t < total; ++t) hn[t] = 0;
        for (size_t x0 = 0; x0 < W; ++x0) {
            for (size_t x1 = 0; x1 < W; ++x1) {
                size_t id = x0 * W + x1;
                if (!hc[id]) continue;
                seq_t base = cur[id];
                size_t xv[2];
                xv[0] = x0; xv[1] = x1;
                for (long long d = 0; d <= i; ++d) {
                    size_t nx[2];
                    for (int b = 0; b < 2; ++b) {
                        size_t s = 0;
                        for (int bi = 0; bi < 2; ++bi) {
                            long long c = d - bi + (i + 1) * b;
                            if (c >= 0 && c <= i + 1) s += xv[bi];
                        }
                        if (s > N + 1) s = N + 1;
                        nx[b] = s;
                    }
                    if (nx[0] == N + 1 && nx[1] == N + 1) continue;
                    size_t nid = nx[0] * W + nx[1];
                    seq_t val = base + (seq_t)d * fac;
                    if (!hn[nid] || val < nxt[nid]) {
                        hn[nid] = 1;
                        nxt[nid] = val;
                    }
                }
            }
        }
        /* swap */
        seq_t *ts = cur; cur = nxt; nxt = ts;
        unsigned char *th = hc; hc = hn; hn = th;
        /* check candidates with x0 == n */
        for (size_t x1 = 0; x1 < W; ++x1) {
            size_t id = N * W + x1;
            if (hc[id]) {
                if (!have_best || cur[id] < best) {
                    best = cur[id];
                    have_best = true;
                }
            }
        }
    }

    mem_free(cur);
    mem_free(nxt);
    mem_free(hc);
    mem_free(hn);
    return have_best ? best : (seq_t)0;
}
