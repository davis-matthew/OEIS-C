/*
 * A291834  First positions of records in A252665.
 * https://oeis.org/A291834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A252665(k) > A252665(j) for all j < k
 *            (first positions of records). Computed by search: A252665
 *            depends only on the prime signature, so only numbers with
 *            non-increasing exponents (A025487) are scanned up to a growing
 *            bound; the count for each is found by a DP over its div...
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 75 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define R_NP 30
static const unsigned r_pr[R_NP] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};

static bool r_fixed(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0) return true;
    }
    return false;
}

static seq_t r_B;
static seq_t *r_arr;
static size_t r_cnt;
static bool r_fill;

static void r_dfs(int pi, int maxe, seq_t val)
{
    int e;
    if (r_fill) r_arr[r_cnt] = val;
    r_cnt++;
    if (pi >= R_NP) return;
    for (e = 1; e <= maxe; e++) {
        if (val > r_B / r_pr[pi]) break;
        val *= r_pr[pi];
        r_dfs(pi + 1, e, val);
    }
}

/* number of ways to write v as a product of 5 factors, unordered (A252665) */
static uint64_t r_count(seq_t v)
{
    int ex[R_NP], r = 0, i, j;
    size_t D = 1, x, d;
    unsigned char *dig;
    uint64_t *dp[6];
    uint64_t res = 0;
    int k;
    if (v == 1) return 1;
    for (i = 0; i < R_NP && v > 1; i++) {
        int e = 0;
        while (v % r_pr[i] == 0) { v /= r_pr[i]; e++; }
        if (e) { ex[r++] = e; D *= (size_t)(e + 1); }
    }
    dig = (unsigned char *)malloc(D * (size_t)r);
    for (x = 0; x < D; x++) {
        size_t t = x;
        for (j = r - 1; j >= 0; j--) {
            dig[x * r + j] = (unsigned char)(t % (size_t)(ex[j] + 1));
            t /= (size_t)(ex[j] + 1);
        }
    }
    for (k = 0; k <= 5; k++) {
        dp[k] = (uint64_t *)calloc(D, sizeof(uint64_t));
    }
    dp[0][0] = 1;
    for (d = 1; d < D; d++) {
        for (k = 1; k <= 5; k++) {
            for (x = d; x < D; x++) {
                bool ok = true;
                for (j = 0; j < r; j++)
                    if (dig[x * r + j] < dig[d * r + j]) { ok = false; break; }
                if (ok) dp[k][x] += dp[k - 1][x - d];
            }
        }
    }
    for (k = 1; k <= 5; k++) res += dp[k][D - 1];
    for (k = 0; k <= 5; k++) free(dp[k]);
    free(dig);
    return res;
}

static seq_t *r_memo = NULL;
static size_t r_memo_len = 0;
static bool r_done = false;

seq_t a(idx_t n)
{
    bool fixed = r_fixed();
    seq_t maxv = 0;
    if (fixed) maxv = (seq_t)0 - 1;
    if (n >= 1 && r_memo != NULL && (size_t)n <= r_memo_len) return r_memo[n - 1];
    if (r_done) return 0;
    for (;;) {
        size_t gap, i, j;
        uint64_t best = 0;
        size_t rc = 0;
        seq_t B;
        if (r_memo != NULL) {
            if (fixed && r_B > maxv / 16) { r_done = true; return 0; }
            B = r_B * 16;
        } else {
            B = 1000;
            if (fixed && B > maxv) B = maxv;
        }
        r_B = B;
        r_fill = false; r_cnt = 0;
        r_dfs(0, 1000, 1);
        r_arr = mem_new(seq_t, r_cnt);
        r_fill = true; r_cnt = 0;
        r_dfs(0, 1000, 1);
        for (gap = r_cnt / 2; gap > 0; gap /= 2) {
            for (i = gap; i < r_cnt; i++) {
                seq_t tmp = r_arr[i];
                j = i;
                while (j >= gap && r_arr[j - gap] > tmp) {
                    r_arr[j] = r_arr[j - gap];
                    j -= gap;
                }
                r_arr[j] = tmp;
            }
        }
        {
            seq_t *recs = mem_new(seq_t, r_cnt);
            for (i = 0; i < r_cnt; i++) {
                uint64_t f = r_count(r_arr[i]);
                if (f > best) { best = f; recs[rc++] = r_arr[i]; }
            }
            if (r_memo != NULL) mem_free(r_memo);
            r_memo = recs;
            r_memo_len = rc;
        }
        mem_free(r_arr);
        r_arr = NULL;
        if (n >= 1 && (size_t)n <= r_memo_len) return r_memo[n - 1];
        if (fixed && r_B >= maxv) { r_done = true; return 0; }
    }
}
