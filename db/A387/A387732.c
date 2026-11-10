/*
 * A387732  Highly powerful numbers that are perfect powers.
 * https://oeis.org/A387732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th number m that is a perfect power (m=1 or gcd of
 *            prime exponents >= 2) and a record-setter of the product of
 *            prime exponents (A005934); found by enumerating all numbers with
 *            non-increasing exponents (A025487) up to a growing limit,
 *            sorting, and scanning for records
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static const int hp_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173};
#define HP_NP 40

static seq_t hp_L;
static seq_t *hp_v;
static uint64_t *hp_pr;
static uint64_t *hp_g;
static size_t hp_cnt;
static bool hp_fill;

static uint64_t hp_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void hp_dfs(int pi, int maxe, seq_t v, uint64_t pr, uint64_t g) {
    if (hp_fill) {
        hp_v[hp_cnt] = v;
        hp_pr[hp_cnt] = pr;
        hp_g[hp_cnt] = g;
    }
    hp_cnt++;
    if (pi >= HP_NP) return;
    int p = hp_primes[pi];
    seq_t lim = hp_L / p;
    for (int e = 1; e <= maxe; e++) {
        if (v > lim) break;
        v *= p;
        hp_dfs(pi + 1, e, v, pr * (uint64_t)e, hp_gcd(g, (uint64_t)e));
    }
}

static int hp_cmp(const void *x, const void *y) {
    size_t i = *(const size_t *)x, j = *(const size_t *)y;
    if (hp_v[i] < hp_v[j]) return -1;
    if (hp_v[j] < hp_v[i]) return 1;
    return 0;
}

seq_t a(idx_t n) {
    seq_t L = 1;
    L <<= 24;
    for (int iter = 0; iter < 60; iter++) {
        hp_L = L;
        hp_cnt = 0;
        hp_fill = false;
        hp_dfs(0, 200, (seq_t)1, 1, 0);
        size_t total = hp_cnt;
        hp_v = mem_new(seq_t, total);
        hp_pr = mem_new(uint64_t, total);
        hp_g = mem_new(uint64_t, total);
        size_t *ord = (size_t *)malloc(total * sizeof(size_t));
        hp_cnt = 0;
        hp_fill = true;
        hp_dfs(0, 200, (seq_t)1, 1, 0);
        for (size_t i = 0; i < total; i++) ord[i] = i;
        qsort(ord, total, sizeof(size_t), hp_cmp);
        uint64_t best = 0;
        idx_t found = 0;
        bool done = false;
        seq_t result = 0;
        for (size_t k = 0; k < total; k++) {
            size_t i = ord[k];
            if (hp_pr[i] > best) {
                best = hp_pr[i];
                if (hp_g[i] != 1) {
                    found++;
                    if (found == n) { result = hp_v[i]; done = true; break; }
                }
            }
        }
        free(ord);
        mem_free(hp_v);
        mem_free(hp_pr);
        mem_free(hp_g);
        if (done) return result;
        seq_t next = L * 256;
        if (next / 256 != L) return 0;
        L = next;
    }
    return 0;
}
