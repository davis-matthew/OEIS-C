/*
 * A302780  Restricted growth sequence transform of 4-tuple [H(H(n-1)),
 *          H(n-H(n-1)), Q(n-Q(n-1)), Q(n-Q(n-2))] where H = A004001 and Q =
 *          A005185.
 * https://oeis.org/A302780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [H(H(n-1)),
 *            H(n-H(n-1)), Q(n-Q(n-1)), Q(n-Q(n-2))], H=A004001, Q=A005185;
 *            a(1)=a(2)=1 (computed by hashing tuples, ids numbered in order
 *            of first appearance from n=3)
 * Range:     u8 n <= 274, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 111 terms through n = 9623 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *g_res = NULL;
static idx_t g_len = 0; /* valid for 1..g_len */

static void A302780_build(idx_t N) {
  idx_t *H = (idx_t *)malloc((N + 2) * sizeof(idx_t));
  idx_t *Q = (idx_t *)malloc((N + 2) * sizeof(idx_t));
  idx_t *T = (idx_t *)malloc(4 * (N + 2) * sizeof(idx_t));
  idx_t *res = (idx_t *)malloc((N + 2) * sizeof(idx_t));
  idx_t hs = 1;
  while (hs < 2 * N + 4) hs <<= 1;
  idx_t *ht = (idx_t *)malloc(hs * sizeof(idx_t)); /* stores index+0, 0 = empty */
  idx_t i, j;
  for (i = 0; i < hs; i++) ht[i] = 0;
  H[0] = 0; Q[0] = 0;
  H[1] = 1; Q[1] = 1;
  if (N >= 2) { H[2] = 1; Q[2] = 1; }
  for (i = 3; i <= N; i++) {
    H[i] = H[H[i - 1]] + H[i - H[i - 1]];
    Q[i] = Q[i - Q[i - 1]] + Q[i - Q[i - 2]];
  }
  res[1] = 1;
  if (N >= 2) res[2] = 1;
  idx_t next_id = 2;
  for (i = 3; i <= N; i++) {
    idx_t t0 = H[H[i - 1]], t1 = H[i - H[i - 1]];
    idx_t t2 = Q[i - Q[i - 1]], t3 = Q[i - Q[i - 2]];
    T[4 * i] = t0; T[4 * i + 1] = t1; T[4 * i + 2] = t2; T[4 * i + 3] = t3;
    uint64_t h = t0 * 1000003ULL + t1;
    h = h * 1000003ULL + t2;
    h = h * 1000003ULL + t3;
    h ^= h >> 29; h *= 0x9E3779B97F4A7C15ULL; h ^= h >> 32;
    idx_t pos = (idx_t)(h & (hs - 1));
    for (;;) {
      j = ht[pos];
      if (j == 0) {
        ht[pos] = i;
        res[i] = next_id++;
        break;
      }
      if (T[4 * j] == t0 && T[4 * j + 1] == t1 && T[4 * j + 2] == t2 && T[4 * j + 3] == t3) {
        res[i] = res[j];
        break;
      }
      pos = (pos + 1) & (hs - 1);
    }
  }
  free(H); free(Q); free(T); free(ht);
  if (g_res) free(g_res);
  g_res = res;
  g_len = N;
}

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (n <= 2) return 1;
  if (n > g_len) {
    idx_t N = n;
    if (N < 2 * g_len) N = 2 * g_len;
    if (N < 1024) N = 1024;
    A302780_build(N);
  }
  return (seq_t)g_res[n];
}
