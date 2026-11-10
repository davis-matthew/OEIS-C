/*
 * A318127  Expansion of (1/(1 - x)) * Product_{k>=1} 1/(1 - k*x^k/(1 - x)^k).
 * https://oeis.org/A318127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A006906(k), where A006906 has
 *            g.f. 1/Product_{k>=1} (1-k*x^k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1984 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A006906_tab = NULL;
static idx_t A006906_len = 0;

static void A006906_build(idx_t len) {
    seq_t *dp = mem_new(seq_t, (size_t)len);
    idx_t k, i;
    for (i = 0; i < len; i++) dp[i] = 0;
    dp[0] = 1;
    for (k = 1; k < len; k++) {
        seq_t kk = (seq_t)k;
        for (i = k; i < len; i++) {
            dp[i] += kk * dp[i - k];
        }
    }
    if (A006906_tab != NULL) {
        for (i = 0; i < A006906_len; i++) A006906_tab[i] = 0;
        mem_free(A006906_tab);
    }
    A006906_tab = dp;
    A006906_len = len;
}

static seq_t A006906_a(idx_t n) {
    if (n >= A006906_len) {
        idx_t len = n + 1 + (n + 1) / 4 + 16;
        A006906_build(len);
    }
    return A006906_tab[n];
}

seq_t a(idx_t n) {
    idx_t i, j;
    seq_t *row;
    seq_t sum = 0;
    (void)A006906_a(n);
    row = mem_new(seq_t, (size_t)(n + 1));
    for (i = 0; i <= n; i++) row[i] = 0;
    row[0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = i; j >= 1; j--) {
            row[j] += row[j - 1];
        }
    }
    for (i = 0; i <= n; i++) {
        sum += row[i] * A006906_a(i);
    }
    for (i = 0; i <= n; i++) row[i] = 0;
    mem_free(row);
    return sum;
}
