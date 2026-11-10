/*
 * A284606  a(n) = (-1)^n * A284019(n).
 * https://oeis.org/A284606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = (-1)^n * (A004001(n) - A005185(n));
 *            A004001(n)=A004001(A004001(n-1))+A004001(n-A004001(n-1)),
 *            a(1)=a(2)=1;
 *            A005185(n)=A005185(n-A005185(n-1))+A005185(n-A005185(n-2)),
 *            a(1)=a(2)=1
 * Range:     i8 n <= 221, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 8982 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A004001_cache = NULL;
static idx_t A004001_len = 0;

static seq_t A004001_a(idx_t n) {
  if (n < 1) return 0;
  if (n >= A004001_len) {
    idx_t new_len = (n < 2) ? 3 : n + 1;
    seq_t *nc = mem_new(seq_t, new_len);
    if (A004001_cache) {
      for (idx_t i = 0; i < A004001_len; i++) nc[i] = A004001_cache[i];
      mem_free(A004001_cache);
    }
    A004001_cache = nc;
    if (A004001_len == 0) {
      A004001_cache[1] = 1;
      A004001_cache[2] = 1;
      A004001_len = 3;
    }
    for (idx_t i = A004001_len; i <= n; i++) {
      idx_t i1 = (idx_t)A004001_cache[i - 1];
      idx_t i2 = i - i1;
      A004001_cache[i] = A004001_cache[i1] + A004001_cache[i2];
    }
    A004001_len = new_len;
  }
  return A004001_cache[n];
}

static seq_t *A005185_memo = NULL;
static idx_t A005185_len = 0;

static seq_t A005185_a(idx_t n) {
  if (n <= 0) return 0;
  if (n <= 2) return 1;
  if (n >= A005185_len) {
    idx_t old_len = A005185_len;
    idx_t new_len = n + 1;
    seq_t *p = mem_new(seq_t, new_len);
    if (A005185_memo) {
      for (idx_t i = 0; i < old_len; i++) p[i] = A005185_memo[i];
      mem_free(A005185_memo);
    }
    A005185_memo = p;
    if (old_len == 0) {
      A005185_memo[1] = 1;
      A005185_memo[2] = 1;
      old_len = 3;
    }
    for (idx_t i = old_len; i <= n; i++) {
      idx_t i1 = i - (idx_t)A005185_memo[i - 1];
      idx_t i2 = i - (idx_t)A005185_memo[i - 2];
      A005185_memo[i] = A005185_memo[i1] + A005185_memo[i2];
    }
    A005185_len = new_len;
  }
  return A005185_memo[n];
}

seq_t a(idx_t n) {
  seq_t d = A004001_a(n) - A005185_a(n);
  if (n & 1) d = -d;
  return d;
}
