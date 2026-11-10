/*
 * A372711  Numbers of minimal total dominating sets in the n-Lucas cube
 *          graph.
 * https://oeis.org/A372711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   brute-force search. L_n is bipartite (even/odd weight strings),
 *            so a minimal total dominating set splits into two independent
 *            parts. Each part is a minimal set of vertices on one side that
 *            dominates the other side, with every chosen vertex having a
 *            private neighbor there. a(n) = T_even * T_odd, w...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int s_nA;
static uint64_t s_nbA[64];
static uint64_t s_remA[65];
static uint64_t s_allB;
static uint64_t s_cnt;
static int s_chosen[64];

static void rec_side(int i, uint64_t once, uint64_t twice, int k) {
    uint64_t cov = once | twice;
    if (cov == s_allB) { s_cnt++; return; }
    if (i == s_nA) return;
    if ((cov | s_remA[i]) != s_allB) return;
    /* include vertex i */
    {
        uint64_t m = s_nbA[i];
        uint64_t fresh = m & ~cov;
        if (fresh) {
            uint64_t no = (once & ~m) | fresh;
            uint64_t nt = twice | (once & m);
            bool ok = true;
            for (int j = 0; j < k; j++) {
                if (!(s_nbA[s_chosen[j]] & no)) { ok = false; break; }
            }
            if (ok) {
                s_chosen[k] = i;
                rec_side(i + 1, no, nt, k + 1);
            }
        }
    }
    /* exclude vertex i */
    rec_side(i + 1, once, twice, k);
}

static int valid_lucas(int x, int nn) {
    if (x & (x >> 1)) return 0;
    if ((x & 1) && ((x >> (nn - 1)) & 1)) return 0;
    return 1;
}

static int par(int x) {
    int p = 0;
    while (x) { p ^= 1; x &= x - 1; }
    return p;
}

/* count minimal sets of side sa dominating side 1-sa */
static uint64_t count_side(int nn, int sa, const int *sideIdx) {
    int total = 1 << nn;
    int nb = 0;
    s_nA = 0;
    for (int x = 0; x < total; x++) {
        if (!valid_lucas(x, nn)) continue;
        if (par(x) == sa) {
            uint64_t m = 0;
            for (int b = 0; b < nn; b++) {
                int y = x ^ (1 << b);
                if (valid_lucas(y, nn)) m |= 1ULL << sideIdx[y];
            }
            s_nbA[s_nA++] = m;
        } else {
            nb++;
        }
    }
    s_allB = (nb >= 64) ? ~0ULL : ((1ULL << nb) - 1);
    s_remA[s_nA] = 0;
    for (int i = s_nA - 1; i >= 0; i--) s_remA[i] = s_remA[i + 1] | s_nbA[i];
    s_cnt = 0;
    rec_side(0, 0, 0, 0);
    return s_cnt;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 10) return (seq_t)0;
    int nn = (int)n;
    int total = 1 << nn;
    int *sideIdx = (int *)malloc(sizeof(int) * (size_t)total);
    int c0 = 0, c1 = 0;
    for (int x = 0; x < total; x++) {
        sideIdx[x] = -1;
        if (!valid_lucas(x, nn)) continue;
        if (par(x) == 0) sideIdx[x] = c0++;
        else sideIdx[x] = c1++;
    }
    if (c0 > 64 || c1 > 64) { free(sideIdx); return (seq_t)0; }
    uint64_t t0 = count_side(nn, 0, sideIdx);
    uint64_t t1 = 0;
    if (t0) t1 = count_side(nn, 1, sideIdx);
    free(sideIdx);
    seq_t r = (seq_t)t0;
    r *= (seq_t)t1;
    return r;
}
