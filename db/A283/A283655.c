/*
 * A283655  a(n) = b(b(n+1)) - b(n-b(n)+1) where b(n) = A004001(n).
 * https://oeis.org/A283655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(b(n+1)) - b(n-b(n)+1), where b = A004001: b(n) =
 *            b(b(n-1)) + b(n-b(n-1)), b(1) = b(2) = 1
 * Range:     u8 n <= 589, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9131 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A283655_c = NULL;
static idx_t A283655_len = 0;

static void A283655_ensure(idx_t m) {
  if (m < A283655_len) return;
  idx_t nl = m + 1;
  if (nl < 3) nl = 3;
  if (A283655_c) mem_free(A283655_c);
  A283655_c = mem_new(seq_t, nl);
  A283655_c[1] = 1;
  A283655_c[2] = 1;
  for (idx_t i = 3; i < nl; i++) {
    idx_t p = (idx_t)A283655_c[i - 1];
    A283655_c[i] = A283655_c[p] + A283655_c[i - p];
  }
  A283655_len = nl;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  A283655_ensure(n + 1);
  idx_t bn1 = (idx_t)A283655_c[n + 1];
  idx_t bn = (idx_t)A283655_c[n];
  seq_t x = A283655_c[bn1];
  seq_t y = A283655_c[n - bn + 1];
  return x - y;
}
