/*
 * A381070  Numbers k such that A380845(k)/k > A380845(m)/m for all m < k.
 * https://oeis.org/A381070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- a(n) are the records of A380845(k)/k. With k = 2^e*m,
 *            m odd, A380845(k) = (2^(e+1)-1)*B(m), where B(m) = Sum_{d|m,
 *            popcount(d)=popcount(m)} d. The code searches odd m by DFS over
 *            prime powers, pruned by the bound sigma(m)/m, over ranges that
 *            grow by a factor of 3, and compares ratios ex...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 40, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/popcount.h"

typedef unsigned __int128 A381070_u128;
typedef struct { uint64_t k, A; } A381070_cd;

static uint64_t *A381070_rec = NULL;
static size_t A381070_nrec = 0, A381070_cap = 0;
static uint64_t A381070_oldN = 0, A381070_N = 0;
static uint64_t A381070_bestA = 0, A381070_bestK = 1;
static uint32_t *A381070_pr = NULL;          /* odd primes */
static size_t A381070_np = 0;
static A381070_cd *A381070_cand = NULL;
static size_t A381070_nc = 0, A381070_ccap = 0;
static long double A381070_thr = 0;
static bool A381070_init = false;
static uint64_t A381070_fp[64], A381070_fe[64];

static void A381070_addrec(uint64_t k) {
    if (A381070_nrec == A381070_cap) {
        A381070_cap = A381070_cap ? 2 * A381070_cap : 256;
        A381070_rec = (uint64_t *)realloc(A381070_rec, A381070_cap * sizeof(uint64_t));
    }
    A381070_rec[A381070_nrec++] = k;
}

static void A381070_consider(uint64_t k, uint64_t A) {
    if ((A381070_u128)A * A381070_bestK > (A381070_u128)A381070_bestA * k) {
        A381070_bestA = A;
        A381070_bestK = k;
        A381070_addrec(k);
    }
}

static void A381070_divs(int depth, int total, uint64_t cur, int pc, uint64_t *sum) {
    if (depth == total) {
        if (popcount(cur) == pc) *sum += cur;
        return;
    }
    uint64_t v = cur;
    for (uint64_t e = 0; e <= A381070_fe[depth]; ++e) {
        A381070_divs(depth + 1, total, v, pc, sum);
        v *= A381070_fp[depth];
    }
}

static void A381070_eval(uint64_t m, long double r, int depth) {
    uint64_t k = m, B = 0;
    int e = 0;
    bool hasB = false;
    while (k <= A381070_N / 2) { k *= 2; e++; }
    for (;;) {
        if (k <= A381070_oldN) break;
        long double f = 2.0L - 1.0L / (long double)((uint64_t)1 << e);
        if (f * r <= A381070_thr) break;
        if (!hasB) {
            A381070_divs(0, depth, 1, popcount(m), &B);
            hasB = true;
        }
        uint64_t A = (((uint64_t)2 << e) - 1) * B;
        if ((long double)A > A381070_thr * (long double)k) {
            if (A381070_nc == A381070_ccap) {
                A381070_ccap = A381070_ccap ? 2 * A381070_ccap : 1024;
                A381070_cand = (A381070_cd *)realloc(A381070_cand,
                                                     A381070_ccap * sizeof(A381070_cd));
            }
            A381070_cand[A381070_nc].k = k;
            A381070_cand[A381070_nc].A = A;
            A381070_nc++;
        }
        if (e == 0) break;
        k /= 2;
        e--;
    }
}

static void A381070_dfs(uint64_t cur, long double r, size_t j, int depth) {
    A381070_eval(cur, r, depth);
    uint64_t M = A381070_N / cur;
    for (size_t jj = j; jj < A381070_np; jj++) {
        uint64_t p = A381070_pr[jj];
        if (p > M) break;
        long double prod = 1;
        uint64_t pp = 1;
        for (size_t t = jj; t < A381070_np && pp <= M / A381070_pr[t]; t++) {
            pp *= A381070_pr[t];
            prod *= (long double)A381070_pr[t] / (long double)(A381070_pr[t] - 1);
        }
        if (2.0L * r * prod <= A381070_thr) break;
        uint64_t pw = cur, a = 0;
        long double sp = 1, q = 1;
        while (pw <= A381070_N / p) {
            pw *= p;
            a++;
            q /= (long double)p;
            sp += q;
            A381070_fp[depth] = p;
            A381070_fe[depth] = a;
            A381070_dfs(pw, r * sp, jj + 1, depth + 1);
        }
    }
}

static int A381070_cmp(const void *x, const void *y) {
    uint64_t a = ((const A381070_cd *)x)->k, b = ((const A381070_cd *)y)->k;
    return a < b ? -1 : (a > b);
}

static void A381070_setup(void) {
    const uint32_t LIM = 1000000;
    char *comp = (char *)calloc(LIM + 1, 1);
    A381070_pr = (uint32_t *)malloc((LIM + 1) * sizeof(uint32_t));
    for (uint32_t i = 2; i <= LIM; i++) {
        if (!comp[i]) {
            if (i > 2) A381070_pr[A381070_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) comp[j] = 1;
        }
    }
    free(comp);
    const uint64_t N0 = 2048;
    A381070_bestA = 0;
    A381070_bestK = 1;
    for (uint64_t k = 1; k <= N0; k++) {
        int pc = popcount(k);
        uint64_t s = 0;
        for (uint64_t d = 1; d <= k; d++)
            if (k % d == 0 && popcount(d) == pc) s += d;
        A381070_consider(k, s);
    }
    A381070_oldN = N0;
    A381070_init = true;
}

static void A381070_stage(void) {
    A381070_N = A381070_oldN * 3;
    A381070_thr = ((long double)A381070_bestA / (long double)A381070_bestK) * (1.0L - 1e-12L);
    A381070_nc = 0;
    A381070_dfs(1, 1.0L, 0, 0);
    qsort(A381070_cand, A381070_nc, sizeof(A381070_cd), A381070_cmp);
    for (size_t c = 0; c < A381070_nc; c++)
        A381070_consider(A381070_cand[c].k, A381070_cand[c].A);
    A381070_oldN = A381070_N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!A381070_init) A381070_setup();
    while (A381070_nrec < (size_t)n) A381070_stage();
    return (seq_t)A381070_rec[n - 1];
}
