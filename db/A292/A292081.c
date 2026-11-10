/*
 * A292081  Let b(k) be A056240(k); this sequence lists numbers b(2n) such
 *          that for some m > n, b(2m) < b(2n).
 * https://oeis.org/A292081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th value b(2m), m>=1, such that b(2k) < b(2m) for some
 *            k > m, where b = A056240. Computed by DP for b(k) (b(k)=k if k
 *            prime, else min over primes p<=k/2 of p*b(k-p)), then a
 *            suffix-minimum scan over the even-index values
 * Range:     u8 none, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 861 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *bt_tab = NULL;
static size_t bt_M = 0;

static void bt_build(size_t M) {
    if (bt_tab != NULL) { mem_free(bt_tab); bt_tab = NULL; }
    bt_tab = mem_new(seq_t, M + 1);
    unsigned char *comp = (unsigned char *)calloc(M + 1, 1);
    uint32_t *pr = (uint32_t *)malloc((M / 2 + 2) * sizeof(uint32_t) + 16);
    size_t pc = 0;
    if (!comp || !pr) abort();
    for (size_t i = 2; i <= M; ++i) {
        if (!comp[i]) {
            if (pc < M / 2 + 2) pr[pc++] = (uint32_t)i;
            for (size_t j = i * i; j <= M && i <= M / i; j += i) comp[j] = 1;
        }
    }
    for (size_t k = 2; k <= M; ++k) {
        if (!comp[k]) { bt_tab[k] = (seq_t)k; continue; }
        seq_t best = 0;
        bool have = false;
        for (size_t i = 0; i < pc; ++i) {
            size_t p = pr[i];
            if (2 * p > k) break;
            seq_t v = (seq_t)p * bt_tab[k - p];
            if (!have || v < best) { best = v; have = true; }
        }
        bt_tab[k] = best;
    }
    free(comp);
    free(pr);
    bt_M = M;
}

/* returns true and sets *out on success */
static bool bt_try(idx_t target, seq_t *out) {
    size_t H = bt_M / 2;
    unsigned char *nr = (unsigned char *)calloc(H + 2, 1);
    if (!nr) abort();
    seq_t minv = 0;
    bool have = false;
    for (size_t n = H; n >= 1; --n) {
        seq_t E = bt_tab[2 * n];
        if (have && minv < E) nr[n] = 1;
        if (!have || E < minv) { minv = E; have = true; }
    }
    seq_t bound = (seq_t)3 * ((seq_t)bt_M - 2);
    idx_t count = 0;
    bool ok = false;
    for (size_t n = 1; n <= H; ++n) {
        seq_t E = bt_tab[2 * n];
        if (nr[n]) {
            ++count;
            if (count == target) { *out = E; ok = true; break; }
        } else if (E > bound) {
            break; /* cannot verify */
        }
    }
    free(nr);
    return ok;
}

seq_t a(idx_t n) {
    size_t want = (size_t)n * 4 + 200;
    if (want < bt_M * 2 && bt_M != 0 && want > bt_M) want = bt_M * 2;
    if (bt_M < want && bt_M < (size_t)n * 4 + 200) bt_build(want);
    for (;;) {
        seq_t r = 0;
        if (bt_try(n, &r)) return r;
        bt_build(bt_M * 2);
    }
}
