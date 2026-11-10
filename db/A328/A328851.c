/*
 * A328851  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^(k + 1).
 * https://oeis.org/A328851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001055(n/d)*A050367(d); computed as the
 *            Dirichlet series Product_{k>=2} (1-k^(-s))^(-(k+1)) restricted
 *            to divisors of n
 * Range:     u8 n <= 23, u16 n <= 575, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Validated: standard, pass, 76 terms through n = 3993 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static size_t dv_find(const idx_t *d, size_t cnt, idx_t v) {
  size_t lo = 0, hi = cnt;
  while (lo < hi) {
    size_t mid = (lo + hi) / 2;
    if (d[mid] < v) lo = mid + 1; else hi = mid;
  }
  return lo;
}

seq_t a(idx_t n) {
  if (n <= 1) return (seq_t)1;
  /* collect divisors in ascending order */
  idx_t *dv = (idx_t *)malloc(sizeof(idx_t) * 2 * (size_t)(1 + 0) + 0);
  free(dv);
  size_t cap = 64, cnt = 0, hcnt = 0;
  idx_t *lowd = (idx_t *)malloc(sizeof(idx_t) * cap);
  idx_t *highd = (idx_t *)malloc(sizeof(idx_t) * cap);
  for (idx_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      if (cnt >= cap || hcnt >= cap) {
        cap *= 2;
        lowd = (idx_t *)realloc(lowd, sizeof(idx_t) * cap);
        highd = (idx_t *)realloc(highd, sizeof(idx_t) * cap);
      }
      lowd[cnt++] = d;
      if (d != n / d) highd[hcnt++] = n / d;
    }
  }
  size_t total = cnt + hcnt;
  idx_t *D = (idx_t *)malloc(sizeof(idx_t) * total);
  for (size_t i = 0; i < cnt; i++) D[i] = lowd[i];
  for (size_t i = 0; i < hcnt; i++) D[cnt + i] = highd[hcnt - 1 - i];
  free(lowd);
  free(highd);

  seq_t *f = mem_new(seq_t, total);
  f[0] = 1;
  for (size_t ki = 1; ki < total; ki++) {
    idx_t k = D[ki];
    idx_t reps = k + 1;
    for (idx_t r = 0; r < reps; r++) {
      for (size_t mi = ki; mi < total; mi++) {
        idx_t m = D[mi];
        if (m % k == 0) {
          size_t qi = dv_find(D, total, m / k);
          f[mi] += f[qi];
        }
      }
    }
  }
  seq_t res = f[total - 1];
  mem_free(f);
  free(D);
  return res;
}
