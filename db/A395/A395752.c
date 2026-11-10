/*
 * A395752  Denominators of the relative increase at positions k given by
 *          A395753, where A000793(k)/A000793(k-1)-1 reaches a new minimum >
 *          0.
 * https://oeis.org/A395752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, hard, more
 * Formula:   search. g(k) = A000793(k) is computed for k <= N by a 0/1
 *            knapsack over primes p <= 1.3*sqrt(N*ln N)+30 (the
 *            Massias-Nicolas-Robin bound on the largest prime of g(k)). The
 *            knapsack keeps log g in long double, an exact additive
 *            fingerprint of the prime-power multiset, and a table of choices
 *            used t...
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 22, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

static seq_t *a395752_res = NULL;
static idx_t a395752_cnt = 0;
static idx_t a395752_rcap = 0;

static uint64_t a395752_mix(uint64_t x) {
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31);
}

static void a395752_push(seq_t v) {
    if (a395752_cnt >= a395752_rcap) {
        idx_t nc = a395752_rcap ? a395752_rcap * 2 : 64;
        if (a395752_res == NULL) a395752_res = mem_new(seq_t, (size_t)nc);
        else a395752_res = mem_resize(seq_t, a395752_res, (size_t)nc);
        a395752_rcap = nc;
    }
    a395752_res[a395752_cnt++] = v;
}

static void a395752_build(long N) {
    long B = (long)(1.3 * sqrt((double)N * log((double)N))) + 30;
    long cap = 4 * B;
    long i, s, k;
    char *comp = (char *)calloc((size_t)(B + 1), 1);
    long *pr = (long *)malloc(sizeof(long) * (size_t)(B + 1));
    int np = 0;
    for (i = 2; i <= B; i++) {
        if (comp[i]) continue;
        pr[np++] = i;
        for (s = i * i; s <= B; s += i) comp[s] = 1;
    }
    free(comp);

    long double *L = (long double *)malloc(sizeof(long double) * (size_t)(N + 1));
    uint64_t *h = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    unsigned char **cb = (unsigned char **)calloc((size_t)np, sizeof(unsigned char *));
    uint64_t **cbit = (uint64_t **)calloc((size_t)np, sizeof(uint64_t *));
    for (s = 0; s <= N; s++) L[s] = 0.0L;

    for (int t = 0; t < np; t++) {
        long p = pr[t];
        long double lp = logl((long double)p);
        if (p * p <= cap) {
            unsigned char *c = (unsigned char *)calloc((size_t)(N + 1), 1);
            cb[t] = c;
            for (s = N; s >= p; s--) {
                long double best = L[s];
                int be = 0;
                long bpk = 1;
                long pk = p;
                int e = 1;
                while (pk <= s && pk <= cap) {
                    long double cnd = L[s - pk] + (long double)e * lp;
                    if (cnd > best) { best = cnd; be = e; bpk = pk; }
                    pk *= p; e++;
                }
                if (be) {
                    L[s] = best;
                    h[s] = h[s - bpk] + a395752_mix((uint64_t)p * 64 + (uint64_t)be);
                    c[s] = (unsigned char)be;
                }
            }
        } else {
            uint64_t *bits = (uint64_t *)calloc((size_t)(N / 64 + 1), sizeof(uint64_t));
            cbit[t] = bits;
            uint64_t f = a395752_mix((uint64_t)p * 64 + 1);
            for (s = N; s >= p; s--) {
                long double cnd = L[s - p] + lp;
                if (cnd > L[s]) {
                    L[s] = cnd;
                    h[s] = h[s - p] + f;
                    bits[s >> 6] |= (uint64_t)1 << (s & 63);
                }
            }
        }
    }

    /* scan for records */
    int *e1 = (int *)calloc((size_t)np, sizeof(int));
    int *e0 = (int *)calloc((size_t)np, sizeof(int));
    const unsigned __int128 LIM = (unsigned __int128)1 << 63;
    bool have = false;
    long double bestd = 0.0L, bestv = 0.0L;
    unsigned __int128 bA = 0, bB = 1;
    bool bovf = false;
    a395752_cnt = 0;

    for (k = 2; k <= N; k++) {
        if (h[k] == h[k - 1]) continue;
        long double d = L[k] - L[k - 1];
        if (have && d > bestd + 1e-12L) continue;
        /* reconstruct exponents of g(k) and g(k-1) */
        for (int r = 0; r < 2; r++) {
            int *ex = r ? e0 : e1;
            long ss = r ? k - 1 : k;
            for (int t = np - 1; t >= 0; t--) {
                int e = 0;
                if (cb[t]) e = cb[t][ss];
                else if (cbit[t]) e = (int)((cbit[t][ss >> 6] >> (ss & 63)) & 1);
                ex[t] = e;
                if (e) {
                    long pk = 1;
                    for (int j = 0; j < e; j++) pk *= pr[t];
                    ss -= pk;
                }
            }
        }
        unsigned __int128 A = 1, Bv = 1;
        long double lA = 1.0L, lB = 1.0L;
        bool ovf = false;
        seq_t den = 1;
        for (int t = 0; t < np; t++) {
            int df = e1[t] - e0[t];
            unsigned __int128 p = (unsigned __int128)pr[t];
            while (df > 0) {
                if (A > LIM / p) ovf = true; else A *= p;
                lA *= (long double)pr[t];
                df--;
            }
            while (df < 0) {
                if (Bv > LIM / p) ovf = true; else Bv *= p;
                lB *= (long double)pr[t];
                den = den * (seq_t)pr[t];
                df++;
            }
        }
        if (!ovf) { if (A <= Bv) continue; }
        else if (lA <= lB) continue;
        long double v = (lA - lB) / lB;
        bool rec;
        if (!have) rec = true;
        else if (!ovf && !bovf) rec = (A - Bv) * bB < (bA - bB) * Bv;
        else rec = v < bestv;
        if (rec) {
            have = true;
            bestd = d; bestv = v;
            bA = A; bB = Bv; bovf = ovf;
            a395752_push(den);
        }
    }

    free(e1); free(e0);
    for (int t = 0; t < np; t++) { free(cb[t]); free(cbit[t]); }
    free(cb); free(cbit);
    free(L); free(h); free(pr);
}

seq_t a(idx_t n) {
    static long N = 0;
    if (n < 1) return 0;
    while (a395752_cnt < n) {
        N = (N == 0) ? 1024 : N * 2;
        a395752_build(N);
    }
    return a395752_res[n - 1];
}
