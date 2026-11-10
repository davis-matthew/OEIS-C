/*
 * A306082  Expansion of e.g.f. Product_{k>=1} 1/(1 - (exp(x) - 1)^(k^2)).
 * https://oeis.org/A306082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * A001156(k) * k!, where
 *            A001156 = partitions into squares
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 416 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A306082_sq_parts(idx_t len) {
    seq_t *dp = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) dp[i] = 0;
    dp[0] = 1;
    for (idx_t m = 1; m * m < len; m++) {
        idx_t sq = m * m;
        for (idx_t i = sq; i < len; i++) dp[i] += dp[i - sq];
    }
    return dp;
}

seq_t a(idx_t n) {
    seq_t *p = A306082_sq_parts(n + 1);
    seq_t *s = mem_new(seq_t, (size_t)(n + 1));
    for (idx_t i = 0; i <= n; i++) s[i] = 0;
    s[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j >= 1; j--) {
            s[j] = (seq_t)j * s[j] + s[j - 1];
        }
        s[0] = 0;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (idx_t k = 0; k <= n; k++) {
        if (k > 0) fact *= (seq_t)k;
        result += s[k] * p[k] * fact;
    }
    mem_free(p);
    mem_free(s);
    return result;
}
