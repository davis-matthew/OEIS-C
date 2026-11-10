/*
 * A283471  Numbers n > 2 such that A004001(A004001(n-1)) =
 *          A004001(n-A004001(n-1)).
 * https://oeis.org/A283471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m > 2 with A004001(A004001(m-1)) =
 *            A004001(m-A004001(m-1)); A004001(n) = A004001(A004001(n-1)) +
 *            A004001(n-A004001(n-1)), A004001(1) = A004001(2) = 1; found by a
 *            linear scan
 * Range:     u8 n <= 39, u16 n <= 147, u32 n <= 185, u64 n <= 185,
 *            u128 n <= 185, big any n
 * Validated: standard, pass, 86 terms through n = 184 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *H_tab = NULL;
static idx_t H_len = 0;   /* valid indices 1..H_len-1 */
static idx_t H_cap = 0;
static idx_t *terms = NULL;
static idx_t terms_cnt = 0;
static idx_t terms_cap = 0;
static idx_t next_m = 3;

static void H_ensure(idx_t n) {
  if (n < H_len) return;
  if (n >= H_cap) {
    idx_t nc = H_cap ? H_cap : 1024;
    while (nc <= n) nc *= 2;
    idx_t *p = (idx_t *)realloc(H_tab, (size_t)nc * sizeof(idx_t));
    H_tab = p;
    H_cap = nc;
  }
  if (H_len == 0) {
    H_tab[0] = 0;
    H_tab[1] = 1;
    H_tab[2] = 1;
    H_len = 3;
  }
  for (idx_t i = H_len; i <= n; i++) {
    idx_t p = H_tab[i - 1];
    H_tab[i] = H_tab[p] + H_tab[i - p];
  }
  H_len = n + 1;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  while (terms_cnt < n) {
    idx_t m = next_m++;
    H_ensure(m);
    idx_t p = H_tab[m - 1];
    if (H_tab[p] == H_tab[m - p]) {
      if (terms_cnt >= terms_cap) {
        idx_t nc = terms_cap ? terms_cap * 2 : 256;
        terms = (idx_t *)realloc(terms, (size_t)nc * sizeof(idx_t));
        terms_cap = nc;
      }
      terms[terms_cnt++] = m;
    }
  }
  return (seq_t)terms[n - 1];
}
