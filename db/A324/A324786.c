/*
 * A324786  First differences of A324784: distances in A005132 from one low
 *          point to the next.
 * https://oeis.org/A324786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Compute Recaman's sequence r = A005132 and find its low
 *            points A324784: k = 0, and every k with r(k-1) > r(k) < r(k+1) <
 *            r(k+2). Then a(n) = A324784(n+1) - A324784(n)
 * Range:     u8 n <= 24, u16 n <= 229, u32 n <= 229, u64 n <= 229,
 *            u128 n <= 229, big any n
 * Validated: standard, pass, 81 terms through n = 212 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *R = NULL;          /* Recaman values r(0..Rlen-1) */
static size_t Rlen = 0, Rcap = 0;
static unsigned char *V = NULL;     /* V[v] = 1 if v already appeared */
static size_t Vcap = 0;

static uint64_t *LP = NULL;         /* low point indices */
static size_t LPlen = 0, LPcap = 0;
static size_t scanned = 1;          /* next k to test */

static void mark_visited(uint64_t v) {
    if (v >= Vcap) {
        size_t nc = Vcap ? Vcap : 1024;
        while (nc <= v) nc *= 2;
        V = (unsigned char *)realloc(V, nc);
        memset(V + Vcap, 0, nc - Vcap);
        Vcap = nc;
    }
    V[v] = 1;
}

static void recaman_extend(size_t m) {
    if (m < Rlen) return;
    if (m + 1 > Rcap) {
        size_t nc = Rcap ? Rcap : 1024;
        while (nc < m + 1) nc *= 2;
        R = (uint64_t *)realloc(R, sizeof(uint64_t) * nc);
        Rcap = nc;
    }
    if (Rlen == 0) {
        R[0] = 0;
        mark_visited(0);
        Rlen = 1;
    }
    for (size_t i = Rlen; i <= m; i++) {
        uint64_t prev = R[i - 1];
        uint64_t step = (uint64_t)i;
        uint64_t cur;
        if (prev >= step) {
            uint64_t c = prev - step;
            bool used = (c < Vcap) && V[c];
            cur = used ? prev + step : c;
        } else {
            cur = prev + step;
        }
        R[i] = cur;
        mark_visited(cur);
    }
    Rlen = m + 1;
}

static void lp_push(uint64_t k) {
    if (LPlen == LPcap) {
        size_t nc = LPcap ? LPcap * 2 : 256;
        LP = (uint64_t *)realloc(LP, sizeof(uint64_t) * nc);
        LPcap = nc;
    }
    LP[LPlen++] = k;
}

seq_t a(idx_t n) {
    if (LPlen == 0) lp_push(0);
    while ((idx_t)LPlen <= n) {
        size_t k = scanned;
        if (k + 2 >= Rlen) {
            size_t target = Rlen * 2;
            if (target < k + 3) target = k + 3;
            if (target < 4096) target = 4096;
            recaman_extend(target);
        }
        if (R[k] < R[k - 1] && R[k + 1] > R[k] && R[k + 2] > R[k + 1])
            lp_push((uint64_t)k);
        scanned++;
    }
    return (seq_t)(LP[n] - LP[n - 1]);
}
