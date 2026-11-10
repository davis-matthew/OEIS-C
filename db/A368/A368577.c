/*
 * A368577  Irregular table T(n, k), n >= 0, k = 1..A036497(n), read by rows;
 *          the n-th row lists the numbers m such that A331835(m) = n.
 * https://oeis.org/A368577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   row n lists, in increasing order, all m with A331835(m) = n,
 *            i.e. n = sum of distinct terms of {1,2,3,5,7,11,...} with bit 0
 *            -> 1 and bit k -> prime(k); a(N) is the N-th entry of the flat
 *            table. Computed by a knapsack count to find the row, then an
 *            ordered DFS
 * Range:     u8 n <= 59, u16 n <= 854, u32 n <= 10888, u64 n <= 10888,
 *            u128 n <= 10888, big any n
 * Validated: standard, pass, 84 terms through n = 9453 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *T_vals = NULL;
static uint64_t *T_pre = NULL;
static uint64_t T_target;
static seq_t T_res;

static bool T_dfs(size_t k, uint64_t rem, seq_t m)
{
    if (rem == 0) {
        if (T_target == 0) { T_res = m; return true; }
        T_target--;
        return false;
    }
    if (k == 0) return false;
    if (T_pre[k] < rem) return false;
    if (T_dfs(k - 1, rem, m)) return true;
    if ((uint64_t)T_vals[k - 1] <= rem) {
        seq_t bit = 1;
        for (size_t i = 0; i + 1 < k; i++) bit = bit * 2;
        if (T_dfs(k - 1, rem - T_vals[k - 1], m + bit)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t L = 16;
    for (;;) {
        bool *comp = (bool *)calloc(L + 1, sizeof(bool));
        uint32_t *vals = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
        size_t K = 0;
        vals[K++] = 1;
        for (uint64_t i = 2; i <= L; i++) {
            if (!comp[i]) {
                vals[K++] = (uint32_t)i;
                for (uint64_t j = i * i; j <= L; j += i) comp[j] = true;
            }
        }
        uint64_t *cnt = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
        cnt[0] = 1;
        for (size_t k = 0; k < K; k++) {
            for (uint64_t s = L; s >= vals[k]; s--) {
                cnt[s] += cnt[s - vals[k]];
                if (s == 0) break;
            }
        }
        uint64_t cum = 0, row = 0;
        bool found = false;
        for (row = 0; row <= L; row++) {
            if (N < cum + cnt[row]) { found = true; break; }
            cum += cnt[row];
        }
        if (found) {
            /* only parts <= row matter */
            size_t KK = 0;
            while (KK < K && vals[KK] <= row) KK++;
            uint64_t *pre = (uint64_t *)calloc(KK + 1, sizeof(uint64_t));
            for (size_t i = 0; i < KK; i++) pre[i + 1] = pre[i] + vals[i];
            T_vals = vals;
            T_pre = pre;
            T_target = N - cum;
            T_res = 0;
            T_dfs(KK, row, (seq_t)0);
            seq_t r = T_res;
            free(pre);
            free(cnt);
            free(vals);
            free(comp);
            T_vals = NULL;
            T_pre = NULL;
            return r;
        }
        free(cnt);
        free(vals);
        free(comp);
        L *= 2;
    }
}
