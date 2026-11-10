/*
 * A283472  a(n) = A004001(A004001(n-1)) AND A004001(n-A004001(n-1)), a(1) =
 *          a(2) = 0.
 * https://oeis.org/A283472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004001(A004001(n-1)) AND A004001(n-A004001(n-1)) for n >
 *            2, a(1) = a(2) = 0; A004001(n) = A004001(A004001(n-1)) +
 *            A004001(n-A004001(n-1)), A004001(1) = A004001(2) = 1
 * Range:     u8 n <= 1014, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9947 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *hc_cache = NULL;
static idx_t hc_len = 0;

static void hc_build(idx_t n) {
  if (n < hc_len) return;
  idx_t new_len = n + 3;
  if (new_len < 16) new_len = 16;
  if (hc_cache) mem_free(hc_cache);
  hc_cache = mem_new(seq_t, new_len);
  hc_cache[1] = 1;
  hc_cache[2] = 1;
  for (idx_t i = 3; i < new_len; i++) {
    idx_t p = (idx_t)hc_cache[i - 1];
    hc_cache[i] = hc_cache[p] + hc_cache[i - p];
  }
  hc_len = new_len;
}

seq_t a(idx_t n) {
  if (n < 3) return 0;
  hc_build(n);
  idx_t p = (idx_t)hc_cache[n - 1];
  seq_t x = hc_cache[p];
  seq_t y = hc_cache[n - p];
  return x & y;
}
