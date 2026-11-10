/*
 * A266348  a(1) = 1; for n > 1, a(n) = A004001(n+1) - A072376(n).
 * https://oeis.org/A266348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1) = 1; for n > 1, a(n) = A004001(n+1) - 2^(floor(log2(n))-1),
 *            with A004001(1) = A004001(2) = 1, A004001(n) =
 *            A004001(A004001(n-1)) + A004001(n-A004001(n-1))
 * Range:     u8 n <= 1012, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 108 terms through n = 7742 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A266348_cache = NULL;
static idx_t A266348_len = 0;

static seq_t A266348_hc(idx_t n) {
  if (n >= A266348_len) {
    idx_t new_len = n + 1;
    if (new_len < 3) new_len = 3;
    seq_t *nc = mem_new(seq_t, new_len);
    idx_t i;
    if (A266348_cache) {
      for (i = 0; i < A266348_len; i++) nc[i] = A266348_cache[i];
      mem_free(A266348_cache);
    }
    A266348_cache = nc;
    idx_t start = A266348_len;
    if (start < 3) {
      nc[1] = 1;
      nc[2] = 1;
      start = 3;
    }
    for (i = start; i < new_len; i++) {
      idx_t p = (idx_t)nc[i - 1];
      nc[i] = nc[p] + nc[i - p];
    }
    A266348_len = new_len;
  }
  return A266348_cache[n];
}

seq_t a(idx_t n) {
  if (n <= 1) return (seq_t)1;
  unsigned k = 0;
  idx_t t = n;
  while (t >>= 1) k++;
  return A266348_hc(n + 1) - ((seq_t)1 << (k - 1));
}
