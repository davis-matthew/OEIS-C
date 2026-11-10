/*
 * A317648  a(1) = a(2) = 1; for n >= 3, a(n) = a(t(n)) + a(n-t(n)) where t =
 *          A004001.
 * https://oeis.org/A317648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(t(n)) + a(n-t(n)) for n >= 3, a(1) = a(2) = 1, where t
 *            = A004001 (t(n) = t(t(n-1)) + t(n-t(n-1)), t(1) = t(2) = 1)
 * Range:     u8 n <= 510, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 99 terms through n = 8999 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A317648_cache = NULL;
static idx_t A317648_len = 0;

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (n >= A317648_len) {
    idx_t new_len = n + 1;
    if (new_len < 2 * A317648_len) new_len = 2 * A317648_len;
    if (new_len < 4) new_len = 4;
    uint64_t *t = (uint64_t *)calloc((size_t)new_len, sizeof(uint64_t));
    seq_t *c = mem_new(seq_t, new_len);
    t[1] = 1; t[2] = 1;
    c[1] = 1; c[2] = 1;
    for (idx_t i = 3; i < new_len; i++) {
      uint64_t p = t[i - 1];
      t[i] = t[p] + t[i - p];
    }
    for (idx_t i = 3; i < new_len; i++) {
      idx_t ti = (idx_t)t[i];
      c[i] = c[ti] + c[i - ti];
    }
    free(t);
    if (A317648_cache) mem_free(A317648_cache);
    A317648_cache = c;
    A317648_len = new_len;
  }
  return A317648_cache[n];
}
