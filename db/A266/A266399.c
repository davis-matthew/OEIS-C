/*
 * A266399  a(n) = A188163(A088359(n)); positions where A004001 obtains unique
 *          values.
 * https://oeis.org/A266399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A004001(m-1) < A004001(m) <
 *            A004001(m+1);
 *            A004001(n)=A004001(A004001(n-1))+A004001(n-A004001(n-1)),
 *            a(1)=a(2)=1
 * Range:     u8 n <= 63, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 84 terms through n = 7707 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *T_terms = NULL;
static idx_t T_count = 0;
static idx_t T_limit = 0;

static void T_build(idx_t L) {
  idx_t *H = (idx_t *)malloc((size_t)(L + 2) * sizeof(idx_t));
  idx_t *res = (idx_t *)malloc((size_t)(L + 2) * sizeof(idx_t));
  idx_t cnt = 0;
  H[0] = 0;
  H[1] = 1;
  H[2] = 1;
  for (idx_t i = 3; i <= L + 1; i++) {
    idx_t p = H[i - 1];
    H[i] = H[p] + H[i - p];
  }
  for (idx_t m = 2; m <= L; m++) {
    if (H[m - 1] < H[m] && H[m] < H[m + 1]) res[cnt++] = m;
  }
  free(H);
  if (T_terms) free(T_terms);
  T_terms = res;
  T_count = cnt;
  T_limit = L;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (T_limit == 0) T_build(256);
  while (T_count < n) T_build(T_limit * 2);
  return (seq_t)T_terms[n - 1];
}
