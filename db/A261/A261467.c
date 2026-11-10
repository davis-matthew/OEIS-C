/*
 * A261467  Where records occur in A261461.
 * https://oeis.org/A261467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(1) = 0; a(n+1) = smallest k whose binary expansion contains
 *            every r = 1..A261461(a(n)) as a substring. k is found by an
 *            exact shortest-path DP over (last K-1 bits, set of uncovered
 *            "leaf" numbers), then the lexicographically least string of
 *            minimal length is rebuilt greedily (search, not closed...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int A261467_bitlen(uint64_t x)
{
    int c = 0;
    while (x) { ++c; x >>= 1; }
    return c;
}

/* does binary of y contain binary of x (x >= 1) as a substring? */
static bool A261467_contains(uint64_t y, uint64_t x)
{
    int lx = A261467_bitlen(x), ly = A261467_bitlen(y), s;
    uint64_t mk;
    if (lx > ly) return false;
    mk = (lx >= 64) ? ~0ULL : ((1ULL << lx) - 1);
    for (s = 0; s <= ly - lx; ++s)
        if (((y >> s) & mk) == x) return true;
    return false;
}

/* smallest nonzero number not a binary substring of g (g >= 1) */
static int A261467_mex(uint64_t g)
{
    int x = 1;
    while (A261467_contains(g, (uint64_t)x)) ++x;
    return x;
}

/* smallest positive integer whose binary contains all of 1..m */
static uint64_t A261467_G(int m)
{
    int K, nodes, x, y, v, b, nl = 0, l;
    int lid[128];
    uint32_t gain[64][2];
    int nxt[64][2];
    uint32_t full;
    long long Mi;
    unsigned char *D;
    int bestT = 1000, bestp = -1;
    uint32_t bestmask = 0;
    uint64_t res;

    if (m == 1) return 1;
    if (m > 120) return 0;
    K = A261467_bitlen((uint64_t)m);
    nodes = 1 << (K - 1);

    for (x = 1; x <= m; ++x) {
        bool leaf = true;
        for (y = x + 1; y <= m && leaf; ++y)
            if (A261467_contains((uint64_t)y, (uint64_t)x)) leaf = false;
        lid[x] = leaf ? nl++ : -1;
    }
    if (nl > 24) return 0;
    full = (nl == 32) ? 0xFFFFFFFFu : ((1u << nl) - 1u);

    for (v = 0; v < nodes; ++v)
        for (b = 0; b < 2; ++b) {
            uint32_t w = ((uint32_t)v << 1) | (uint32_t)b;
            uint32_t gm = 0;
            for (l = 1; l <= K; ++l) {
                uint32_t xv = w & ((1u << l) - 1u);
                if (((xv >> (l - 1)) & 1u) && xv <= (uint32_t)m && lid[xv] >= 0)
                    gm |= 1u << lid[xv];
            }
            gain[v][b] = gm;
            nxt[v][b] = (int)(w & (uint32_t)(nodes - 1));
        }

    D = (unsigned char *)malloc(((size_t)full + 1) * (size_t)nodes);
    if (!D) return 0;
    for (v = 0; v < nodes; ++v) D[(size_t)full * nodes + v] = 0;

    for (Mi = (long long)full - 1; Mi >= 0; --Mi) {
        uint32_t M = (uint32_t)Mi;
        unsigned char *row = D + (size_t)M * nodes;
        int ch;
        for (v = 0; v < nodes; ++v) {
            int best = 255;
            for (b = 0; b < 2; ++b) {
                uint32_t gm = gain[v][b];
                if (gm & ~M) {
                    int c = (int)D[(size_t)(M | gm) * nodes + nxt[v][b]] + 1;
                    if (c < best) best = c;
                }
            }
            row[v] = (unsigned char)best;
        }
        do {
            ch = 0;
            for (v = 0; v < nodes; ++v)
                for (b = 0; b < 2; ++b)
                    if (!(gain[v][b] & ~M)) {
                        int c = (int)row[nxt[v][b]] + 1;
                        if (c < (int)row[v]) { row[v] = (unsigned char)c; ch = 1; }
                    }
        } while (ch);
    }

    /* enumerate the first K-1 bits (leading 1) */
    for (v = nodes >> 1; v < nodes; ++v) {
        uint32_t mk = 0;
        int dd, T;
        for (x = 1; x <= m; ++x)
            if (lid[x] >= 0 && A261467_bitlen((uint64_t)x) <= K - 1 &&
                A261467_contains((uint64_t)v, (uint64_t)x))
                mk |= 1u << lid[x];
        dd = D[(size_t)mk * nodes + v];
        if (dd >= 255) continue;
        T = (K - 1) + dd;
        if (T < bestT) { bestT = T; bestp = v; bestmask = mk; }
    }
    if (bestp < 0) { free(D); return 0; }

    {
        uint32_t M = bestmask;
        int cv = bestp;
        int rem = D[(size_t)M * nodes + cv];
        res = (uint64_t)bestp;
        while (rem > 0) {
            for (b = 0; b < 2; ++b) {
                uint32_t nM = M | gain[cv][b];
                int nv = nxt[cv][b];
                if ((int)D[(size_t)nM * nodes + nv] == rem - 1) {
                    res = (res << 1) | (uint64_t)b;
                    M = nM; cv = nv; --rem;
                    break;
                }
            }
            if (b == 2) { free(D); return 0; }
        }
    }
    free(D);
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t vals[80];
    static int cnt = 0;
    static int rec = 1;
    static bool stuck = false;
    if (cnt == 0) { vals[0] = 0; cnt = 1; rec = 1; }
    while ((idx_t)cnt < n && cnt < 80 && !stuck) {
        uint64_t g = A261467_G(rec);
        if (g == 0) { stuck = true; break; }
        vals[cnt++] = g;
        rec = A261467_mex(g);
    }
    if (n < 1 || (idx_t)cnt < n) return (seq_t)0;
    return (seq_t)vals[n - 1];
}
