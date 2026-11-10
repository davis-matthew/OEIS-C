/*
 * A350032  Write n as n = k1 + k2 + ... + km, so that all k are distinct
 *          positive integers and b = A001055(k1) + A001055(k2) + ... +
 *          A001055(km) becomes maximal. a(n) is the number of such partitions
 *          which attain this maximum.
 * https://oeis.org/A350032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts maximizing
 *            Sum A001055(k_i); computed by a 0/1 knapsack DP tracking (max
 *            value, count), with A001055 via a divisor sieve
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
  size_t N = (size_t)n;
  long long *g = (long long *)malloc((N + 1) * sizeof(long long));
  long long *best = (long long *)malloc((N + 1) * sizeof(long long));
  seq_t *cnt = mem_new(seq_t, N + 1);
  size_t i, d, m, s, k;
  for (i = 0; i <= N; i++) { g[i] = 0; best[i] = -1; }
  g[1] = 1;
  for (d = 2; d <= N; d++)
    for (m = d; m <= N; m += d) g[m] += g[m / d];
  best[0] = 0;
  cnt[0] = 1;
  for (k = 1; k <= N; k++) {
    for (s = N; s >= k; s--) {
      if (best[s - k] >= 0) {
        long long v = best[s - k] + g[k];
        if (v > best[s]) {
          best[s] = v;
          cnt[s] = cnt[s - k];
        } else if (v == best[s]) {
          cnt[s] += cnt[s - k];
        }
      }
    }
  }
  seq_t r = cnt[N];
  free(g);
  free(best);
  mem_free(cnt);
  return r;
}
