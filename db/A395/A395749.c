/*
 * A395749  a(n) is the position of the start of the first occurrence of
 *          exactly n consecutive equal terms in A000793.
 * https://oeis.org/A395749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that A000793(k) = ... = A000793(k+n-1) are
 *            equal, A000793(k-1) and A000793(k+n) differ (offset-0 index);
 *            computed by a search: Landau's g via a knapsack over primes (log
 *            values pick the maximum, modular hashes test equality), then a
 *            scan for runs
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static double *r793_lg = NULL;
static uint64_t *r793_h1 = NULL;
static uint32_t *r793_h2 = NULL;
static idx_t r793_N = 0;

static void r793_build(idx_t N) {
    const uint64_t M2 = 4294967291ULL;
    double *lg = (double *)malloc(sizeof(double) * (size_t)(N + 1));
    uint64_t *h1 = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));
    uint32_t *h2 = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(N + 1));
    double dN = (double)N;
    idx_t P = (idx_t)(2.5 * sqrt(dN * log(dN))) + 50;
    if (P > N) P = N;
    bool *comp = (bool *)calloc((size_t)(P + 1), sizeof(bool));
    idx_t s, p, i;
    for (s = 0; s <= N; s++) { lg[s] = 0.0; h1[s] = 1; h2[s] = 1; }
    for (p = 2; p <= P; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= P; i += p) comp[i] = true;
        for (s = N; s >= p; s--) {
            double best = lg[s];
            idx_t bk = 0;
            idx_t pk = p;
            double lpk = log((double)p);
            double cur = lpk;
            while (pk <= s) {
                double cand = lg[s - pk] + cur;
                if (cand > best) { best = cand; bk = pk; }
                if (pk > N / p) break;
                pk *= p;
                cur += lpk;
            }
            if (bk) {
                lg[s] = best;
                h1[s] = h1[s - bk] * (uint64_t)bk;
                h2[s] = (uint32_t)(((uint64_t)h2[s - bk] * ((uint64_t)bk % M2)) % M2);
            }
        }
    }
    free(comp);
    if (r793_lg) { free(r793_lg); free(r793_h1); free(r793_h2); }
    r793_lg = lg; r793_h1 = h1; r793_h2 = h2;
    r793_N = N;
}

static bool r793_eq(idx_t i, idx_t j) {
    return r793_h1[i] == r793_h1[j] && r793_h2[i] == r793_h2[j];
}

seq_t a(idx_t n) {
    idx_t N = r793_N ? r793_N : 4096;
    for (;;) {
        if (r793_N < N) r793_build(N);
        idx_t i = 0;
        while (i < r793_N) {
            idx_t j = i + 1;
            while (j < r793_N && r793_eq(i, j)) j++;
            if (j >= r793_N) break;
            if (j - i == n) return (seq_t)i;
            i = j;
        }
        if (N >= ((idx_t)1 << 23)) return 0;
        N *= 2;
    }
}
