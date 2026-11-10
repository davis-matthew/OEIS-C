/*
 * A351229  Numbers k for which A003415(k) >= A276086(k) > k, where A003415 is
 *          the arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A351229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: enumerate k by primorial-base digits (A049345) with
 *            pruning, keep k where A003415(k) >= A276086(k) > k, sorted
 *            ascending; a(n) = n-th such k
 * Range:     u8 none, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t Q_PR[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t Q_W[17];
static uint64_t Q_L, Q_BOUND;
static uint64_t *Q_res = NULL;
static size_t Q_cnt = 0, Q_cap = 0;
static uint64_t Q_done_L = 0;

static uint64_t q_deriv(uint64_t k) {
    if (k < 2) return 0;
    uint64_t res = 0, rem = k;
    for (uint64_t p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e) res += (k / p) * (uint64_t)e;
    }
    if (rem > 1) res += k / rem;
    return res;
}

static void q_dfs(int i, uint64_t kpart, uint64_t prod) {
    if (i < 0) {
        if (kpart >= 1 && prod > kpart && q_deriv(kpart) >= prod) {
            if (Q_cnt == Q_cap) {
                Q_cap = Q_cap ? Q_cap * 2 : 64;
                Q_res = (uint64_t *)realloc(Q_res, Q_cap * sizeof(uint64_t));
                if (!Q_res) abort();
            }
            Q_res[Q_cnt++] = kpart;
        }
        return;
    }
    uint64_t p = Q_PR[i];
    uint64_t np = prod;
    for (uint64_t d = 0; d < p; d++) {
        if (d > 0) {
            if (np > Q_BOUND / p) break;
            np *= p;
        }
        uint64_t nk = kpart + d * Q_W[i];
        if (nk > Q_L) break;
        uint64_t kmax = nk + Q_W[i] - 1;
        if (kmax > Q_L) kmax = Q_L;
        if (np > kmax * 64) continue;
        q_dfs(i - 1, nk, np);
    }
}

static int q_cmp(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y);
}

static void q_run(uint64_t L) {
    Q_L = L;
    Q_BOUND = L * 64;
    Q_cnt = 0;
    Q_W[0] = 1;
    int top = 0;
    for (int i = 1; i < 16; i++) {
        Q_W[i] = Q_W[i - 1] * Q_PR[i - 1];
        if (Q_W[i] <= L) top = i;
        else break;
    }
    q_dfs(top, 0, 1);
    qsort(Q_res, Q_cnt, sizeof(uint64_t), q_cmp);
    Q_done_L = L;
}

seq_t a(idx_t n) {
    if (Q_done_L == 0) q_run((uint64_t)1 << 20);
    while (Q_cnt < (size_t)n && Q_done_L < ((uint64_t)1 << 52))
        q_run(Q_done_L * 4);
    if (n == 0 || (size_t)n > Q_cnt) return 0;
    return (seq_t)Q_res[(size_t)n - 1];
}
