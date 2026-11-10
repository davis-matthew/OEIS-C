/*
 * A356974  Irregular triangle T(n, k) read by rows, n >= 0, k =
 *          1..A117546(n); the n-th row contains the numbers m such that
 *          A356964(m) = n, in increasing order.
 * https://oeis.org/A356974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) = n-th term of the rows T(r,.), where row r lists, in
 *            increasing order, the m with A356964(m) = r (m's binary digit k
 *            <-> tribonacci(k+3)); found by counting representations per row,
 *            then enumerating the row's subsets of {1,2,4,7,13,...} summing
 *            to r
 * Range:     u8 n <= 221, u16 n <= 7021, u32 n <= 7021, u64 n <= 7021,
 *            u128 n <= 7021, big any n
 * Validated: standard, pass, 86 terms through n = 6826 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t tr_vals[80];
static idx_t tr_pref[80];
static idx_t *tr_out;
static idx_t tr_cnt;

static void tr_rec(int i, idx_t rem, idx_t mask) {
    if (rem == 0) {
        tr_out[tr_cnt++] = mask;
        return;
    }
    if (i < 0) return;
    if (rem > tr_pref[i]) return;
    if (tr_vals[i] <= rem) tr_rec(i - 1, rem - tr_vals[i], mask | ((idx_t)1 << i));
    tr_rec(i - 1, rem, mask);
}

seq_t a(idx_t n) {
    /* tribonacci values 1,2,4,7,13,... up to bound n */
    int K = 0;
    {
        idx_t x = 1, y = 2, z = 4;
        tr_vals[K++] = x;
        if (y <= n) tr_vals[K++] = y;
        if (z <= n && K == 2) tr_vals[K++] = z;
        while (K >= 3) {
            idx_t nx = x + y + z;
            if (nx > n || nx < z) break;
            tr_vals[K++] = nx;
            x = y; y = z; z = nx;
        }
    }
    idx_t *dp = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
    if (!dp) return 0;
    dp[0] = 1;
    for (int k = 0; k < K; ++k) {
        idx_t t = tr_vals[k];
        for (idx_t s = n; s >= t; --s) {
            dp[s] += dp[s - t];
            if (s == 0) break;
        }
    }
    idx_t row = 0, rem = n;
    while (row <= n && dp[row] <= rem) {
        rem -= dp[row];
        ++row;
    }
    idx_t cnt = dp[row <= n ? row : n];
    free(dp);

    idx_t acc = 0;
    for (int k = 0; k < K; ++k) {
        acc += tr_vals[k];
        tr_pref[k] = acc;
    }
    tr_out = (idx_t *)malloc(sizeof(idx_t) * ((size_t)cnt + 1));
    if (!tr_out) return 0;
    tr_cnt = 0;
    int top = K - 1;
    while (top >= 0 && tr_vals[top] > row) --top;
    tr_rec(top, row, 0);
    for (idx_t i = 1; i < tr_cnt; ++i) {
        idx_t v = tr_out[i];
        idx_t j = i;
        while (j > 0 && tr_out[j - 1] > v) {
            tr_out[j] = tr_out[j - 1];
            --j;
        }
        tr_out[j] = v;
    }
    idx_t res = (rem < tr_cnt) ? tr_out[rem] : 0;
    free(tr_out);
    tr_out = NULL;
    return (seq_t)res;
}
