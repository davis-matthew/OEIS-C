/*
 * A350029  Write n as n = k1 + k2 + ... + km, so that all k are distinct
 *          positive integers. a(n) is the maximum value of A001055(k1) +
 *          A001055(k2) + ... + A001055(km) over all such partitions.
 * https://oeis.org/A350029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over sets of distinct positive integers k1+...+km = n
 *            of Sum A001055(ki); computed by 0/1 knapsack DP with exact sum n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
  size_t N = (size_t)n;
  seq_t *f = mem_new(seq_t, N + 1);
  seq_t *dp = mem_new(seq_t, N + 1);
  bool *ok = mem_new(bool, N + 1);
  size_t i, d, j, s;
  f[1] = 1;
  for (d = 2; d <= N; d++) {
    for (j = d; j <= N; j++) {
      if (j % d == 0) f[j] += f[j / d];
    }
  }
  ok[0] = true;
  dp[0] = 0;
  for (i = 1; i <= N; i++) {
    for (s = N; s >= i; s--) {
      if (ok[s - i]) {
        seq_t cand = dp[s - i] + f[i];
        if (!ok[s] || cand > dp[s]) {
          dp[s] = cand;
          ok[s] = true;
        }
      }
    }
  }
  seq_t res = dp[N];
  mem_free(f);
  mem_free(dp);
  mem_free(ok);
  return res;
}
