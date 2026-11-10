/*
 * A391690  Number of congruences of the lattice of alternating sign matrices
 *          of size n such that minima of congruence classes are permutations.
 * https://oeis.org/A391690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of independent sets (equivalently vertex covers)
 *            of the graph G_n whose vertices are the C(n+1,3)
 *            join-irreducibles of ASM_n (bigrassmannians, indexed by (i,j,k)
 *            with 1<=k<=min(i,j,n-i,n-j)), two vertices being adjacent when
 *            their join in ASM_n is not a permutation matrix. Computed...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef unsigned __int128 m128_t;

static int NV;
static m128_t adjm[128];

#define HBITS 19
#define HSIZE (1u << HBITS)
static m128_t *hkeys = 0;
static unsigned char *hused = 0;
static seq_t *hvals = 0;

static int pc128(m128_t x) {
    return __builtin_popcountll((uint64_t)x) + __builtin_popcountll((uint64_t)(x >> 64));
}
static int low128(m128_t x) {
    uint64_t lo = (uint64_t)x;
    if (lo) return __builtin_ctzll(lo);
    return 64 + __builtin_ctzll((uint64_t)(x >> 64));
}
static uint32_t hash128(m128_t x) {
    uint64_t lo = (uint64_t)x, hi = (uint64_t)(x >> 64);
    uint64_t h = lo * 0x9E3779B97F4A7C15ULL ^ (hi + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    return (uint32_t)(h >> (64 - HBITS));
}

static seq_t cnt(m128_t S) {
    m128_t one = 1;
    if (!S) return (seq_t)1;
    int s = low128(S);
    m128_t comp = one << s, frontier = comp;
    while (frontier) {
        int u = low128(frontier);
        frontier &= ~(one << u);
        m128_t nb = adjm[u] & S & ~comp;
        comp |= nb;
        frontier |= nb;
    }
    if (comp != S) {
        seq_t x = cnt(comp);
        seq_t y = cnt(S & ~comp);
        return x * y;
    }
    int sz = pc128(S);
    if (sz == 1) return (seq_t)2;
    uint32_t h = 0;
    bool usememo = sz >= 6;
    if (usememo) {
        h = hash128(S);
        for (int t = 0; t < 64; t++) {
            uint32_t p = (h + (uint32_t)t) & (HSIZE - 1);
            if (!hused[p]) break;
            if (hkeys[p] == S) return hvals[p];
        }
    }
    int best = -1, bd = -1;
    m128_t T = S;
    while (T) {
        int v = low128(T);
        T &= ~(one << v);
        int d = pc128(adjm[v] & S);
        if (d > bd) { bd = d; best = v; }
    }
    seq_t r = cnt(S & ~(one << best));
    r += cnt(S & ~(adjm[best] | (one << best)));
    if (usememo) {
        for (int t = 0; t < 64; t++) {
            uint32_t p = (h + (uint32_t)t) & (HSIZE - 1);
            if (!hused[p]) {
                hused[p] = 1; hkeys[p] = S; hvals[p] = r;
                break;
            }
        }
    }
    return r;
}

static int imin(int a, int b) { return a < b ? a : b; }
static int imax(int a, int b) { return a > b ? a : b; }

seq_t a(idx_t nn) {
    int n = (int)nn;
    if (n <= 1) return (seq_t)1;
    int N1 = n + 1, M = N1 * N1;
    /* enumerate join-irreducibles */
    int cap = 0;
    for (int i = 1; i <= n - 1; i++)
        for (int j = 1; j <= n - 1; j++)
            cap += imin(imin(i, j), imin(n - i, n - j));
    if (cap > 128) return (seq_t)0; /* beyond supported range */
    NV = cap;
    int *R = (int *)malloc(sizeof(int) * (size_t)(cap > 0 ? cap : 1) * (size_t)M);
    int idx = 0;
    for (int i = 1; i <= n - 1; i++)
        for (int j = 1; j <= n - 1; j++) {
            int km = imin(imin(i, j), imin(n - i, n - j));
            for (int k = 1; k <= km; k++) {
                int c = imin(i, j) - k;
                int *r = R + (size_t)idx * M;
                for (int x = 0; x <= n; x++)
                    for (int y = 0; y <= n; y++) {
                        int v1 = imin(x, y);
                        int v2 = c + imax(0, x - i) + imax(0, y - j);
                        r[x * N1 + y] = imin(v1, v2);
                    }
                idx++;
            }
        }
    for (int u = 0; u < NV; u++) adjm[u] = 0;
    int *rm = (int *)malloc(sizeof(int) * (size_t)M);
    m128_t one = 1;
    for (int u = 0; u < NV; u++)
        for (int v = u + 1; v < NV; v++) {
            int *ru = R + (size_t)u * M, *rv = R + (size_t)v * M;
            for (int t = 0; t < M; t++) rm[t] = imin(ru[t], rv[t]);
            bool perm = true;
            for (int x = 1; x <= n && perm; x++)
                for (int y = 1; y <= n; y++) {
                    int e = rm[x * N1 + y] - rm[(x - 1) * N1 + y] - rm[x * N1 + y - 1]
                            + rm[(x - 1) * N1 + y - 1];
                    if (e < 0) { perm = false; break; }
                }
            if (!perm) {
                adjm[u] |= one << v;
                adjm[v] |= one << u;
            }
        }
    free(rm);
    free(R);
    if (!hkeys) {
        hkeys = (m128_t *)malloc(sizeof(m128_t) * HSIZE);
        hused = (unsigned char *)malloc(HSIZE);
        hvals = mem_new(seq_t, HSIZE);
    }
    memset(hused, 0, HSIZE);
    m128_t all = 0;
    for (int u = 0; u < NV; u++) all |= one << u;
    return cnt(all);
}
