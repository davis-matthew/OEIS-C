/*
 * A283677  a(n) = lcm(b(b(n)), b(n-b(n)+1)) where b(n) = A004001(n).
 * https://oeis.org/A283677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lcm(b(b(n)), b(n-b(n)+1)) where b = A004001, b(n) =
 *            b(b(n-1)) + b(n-b(n-1)), b(1) = b(2) = 1
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9776 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t *A283677_tab = NULL;
static idx_t A283677_len = 0;

static void A283677_grow(idx_t n) {
  if (n < A283677_len) return;
  idx_t new_len = n + 1;
  if (new_len < 3) new_len = 3;
  idx_t *t = (idx_t *)calloc((size_t)new_len, sizeof(idx_t));
  for (idx_t i = 0; i < A283677_len; i++) t[i] = A283677_tab[i];
  free(A283677_tab);
  A283677_tab = t;
  idx_t start = A283677_len;
  if (start < 3) {
    t[1] = 1;
    t[2] = 1;
    start = 3;
  }
  for (idx_t i = start; i < new_len; i++) {
    idx_t p = t[i - 1];
    t[i] = t[p] + t[i - p];
  }
  A283677_len = new_len;
}

static idx_t A283677_b(idx_t n) {
  A283677_grow(n);
  return A283677_tab[n];
}

static idx_t A283677_gcd(idx_t x, idx_t y) {
  while (y) {
    idx_t t = x % y;
    x = y;
    y = t;
  }
  return x;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  idx_t bn = A283677_b(n);
  idx_t x = A283677_b(bn);
  idx_t y = A283677_b(n - bn + 1);
  idx_t g = A283677_gcd(x, y);
  return (seq_t)(x / g) * (seq_t)y;
}
