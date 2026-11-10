/*
 * A317921  a(1) = a(2) = 1; for n >= 3, a(n) = 3*a(t(n)) - a(n-t(n)) where t
 *          = A004001.
 * https://oeis.org/A317921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 3*a(t(n)) - a(n-t(n)), a(1) = a(2) = 1, where t =
 *            A004001: t(n) = t(t(n-1)) + t(n-t(n-1)), t(1) = t(2) = 1
 * Range:     i8 n <= 64, i16 n <= 1034, i32 n <= 32768, i64 n <= 32768,
 *            i128 n <= 32768, big any n
 * Validated: standard, pass, 91 terms through n = 9586 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t *A317921_t = NULL;
static seq_t *A317921_v = NULL;
static idx_t A317921_len = 0;

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (n >= A317921_len) {
    idx_t new_len = n + 1;
    if (new_len < 3) new_len = 3;
    if (A317921_t) free(A317921_t);
    if (A317921_v) mem_free(A317921_v);
    A317921_t = (idx_t *)malloc((size_t)new_len * sizeof(idx_t));
    A317921_v = mem_new(seq_t, new_len);
    A317921_t[0] = 0;
    A317921_t[1] = 1;
    A317921_t[2] = 1;
    A317921_v[1] = 1;
    A317921_v[2] = 1;
    for (idx_t i = 3; i < new_len; i++) {
      idx_t p = A317921_t[i - 1];
      A317921_t[i] = A317921_t[p] + A317921_t[i - p];
      idx_t ti = A317921_t[i];
      A317921_v[i] = (seq_t)3 * A317921_v[ti] - A317921_v[i - ti];
    }
    A317921_len = new_len;
  }
  return A317921_v[n];
}
