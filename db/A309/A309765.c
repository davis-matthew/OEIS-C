/*
 * A309765  a(n) is the least k>=1 such that k*(k+1) is divisible by
 *          A003418(n).
 * https://oeis.org/A309765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over nonempty sets S of prime powers q || A003418(n)
 *            of the k in [1, A003418(n)) with k == -1 (mod q) for q in S and
 *            k == 0 (mod q) otherwise, found by a meet-in-the-middle search
 *            over the CRT subset sums
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 167 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Generic search body over a value type T (ALLOC/FREE give its array allocator). */
#define A309765_DEFINE(SUFFIX, T, ALLOC, FREE)                                      \
static T *a309765_build_##SUFFIX(const T *ds, int cnt, T L) {                      \
    size_t sz = 1;                                                                  \
    T *cur = ALLOC(((size_t)1) << cnt);                                             \
    T *nxt = ALLOC(((size_t)1) << cnt);                                             \
    cur[0] = 0;                                                                     \
    for (int i = 0; i < cnt; i++) {                                                 \
        T d = ds[i];                                                                \
        T lim = L - d;                                                              \
        size_t lo = 0, hi = sz;                                                     \
        while (lo < hi) {                                                           \
            size_t mid = lo + (hi - lo) / 2;                                        \
            if (cur[mid] >= lim) hi = mid; else lo = mid + 1;                       \
        }                                                                           \
        size_t t = lo;                                                              \
        size_t wrapn = sz - t;                                                      \
        size_t ia = 0, ib = 0, o = 0;                                               \
        while (ia < sz && ib < sz) {                                                \
            T sv;                                                                   \
            if (ib < wrapn) sv = cur[t + ib] - lim;                                 \
            else sv = cur[ib - wrapn] + d;                                          \
            if (cur[ia] <= sv) { nxt[o++] = cur[ia]; ia++; }                        \
            else { nxt[o++] = sv; ib++; }                                           \
        }                                                                           \
        while (ia < sz) { nxt[o++] = cur[ia]; ia++; }                               \
        while (ib < sz) {                                                           \
            T sv;                                                                   \
            if (ib < wrapn) sv = cur[t + ib] - lim;                                 \
            else sv = cur[ib - wrapn] + d;                                          \
            nxt[o++] = sv; ib++;                                                    \
        }                                                                           \
        sz *= 2;                                                                    \
        T *tmp = cur; cur = nxt; nxt = tmp;                                         \
    }                                                                               \
    FREE(nxt);                                                                      \
    return cur;                                                                     \
}                                                                                   \
static T a309765_solve_##SUFFIX(const long long *qs, int m) {                       \
    T L = 1;                                                                        \
    for (int i = 0; i < m; i++) L = L * (T)qs[i];                                   \
    T *ds = ALLOC((size_t)m);                                                       \
    for (int i = 0; i < m; i++) {                                                   \
        long long q = qs[i];                                                        \
        T M = L / (T)q;                                                             \
        long long r = (long long)(M % (T)q);                                        \
        long long inv = 1;                                                          \
        for (long long x = 1; x < q; x++) if ((r * x) % q == 1) { inv = x; break; } \
        T c = M * (T)inv;                                                           \
        ds[i] = L - c;                                                              \
    }                                                                               \
    int h1 = m / 2, h2 = m - h1;                                                    \
    T *P = a309765_build_##SUFFIX(ds, h1, L);                                       \
    T *Q = a309765_build_##SUFFIX(ds + h1, h2, L);                                  \
    size_t np = ((size_t)1) << h1, nq = ((size_t)1) << h2;                          \
    bool have = false;                                                              \
    T best = 0;                                                                     \
    if (np > 1) { best = P[1]; have = true; }                                       \
    if (nq > 1 && (!have || Q[1] < best)) { best = Q[1]; have = true; }             \
    size_t j = nq;                                                                  \
    for (size_t i = 0; i < np; i++) {                                               \
        T need = L - P[i];                                                          \
        while (j > 0 && Q[j - 1] >= need) j--;                                      \
        if (j < nq) {                                                               \
            T cand = Q[j] - need;                                                   \
            if (cand > 0 && (!have || cand < best)) { best = cand; have = true; }   \
        }                                                                           \
    }                                                                               \
    FREE(P);                                                                        \
    FREE(Q);                                                                        \
    FREE(ds);                                                                       \
    return best;                                                                    \
}

#define A309765_ALLOC_SEQ(cnt) mem_new(seq_t, (cnt))
#define A309765_FREE_SEQ(p) mem_free(p)
#define A309765_ALLOC_U128(cnt) ((unsigned __int128 *)calloc((cnt) ? (cnt) : 1, sizeof(unsigned __int128)))
#define A309765_FREE_U128(p) free(p)

A309765_DEFINE(seq, seq_t, A309765_ALLOC_SEQ, A309765_FREE_SEQ)
A309765_DEFINE(u128, unsigned __int128, A309765_ALLOC_U128, A309765_FREE_U128)

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    int nn = (int)n;
    long long qs[128];
    int m = 0;
    for (int p = 2; p <= nn; p++) {
        bool pr = true;
        for (int d = 2; d * d <= p; d++) if (p % d == 0) { pr = false; break; }
        if (!pr) continue;
        long long q = p;
        while (q * p <= nn) q *= p;
        qs[m++] = q;
    }
    if (n <= 88) {
        /* lcm(1..n) fits in 128 bits: do the whole search there */
        unsigned __int128 r = a309765_solve_u128(qs, m);
        uint64_t hi = (uint64_t)(r >> 64);
        uint64_t lo = (uint64_t)r;
        seq_t res = (seq_t)hi;
        res = res * (seq_t)4294967296ULL;
        res = res * (seq_t)4294967296ULL;
        res = res + (seq_t)lo;
        return res;
    }
    return a309765_solve_seq(qs, m);
}
