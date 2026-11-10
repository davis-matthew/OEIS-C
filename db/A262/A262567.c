/*
 * A262567  a(n) = A002703(n)/2.
 * https://oeis.org/A262567
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002703(n)/2 = (N(n)-2)/2, where N(n) = (1/(2n+1)) *
 *            Sum_{d | 2n+1} phi(d)*(2/d)*2^(((2n+1)/d-1)/2), (2/d) the Jacobi
 *            symbol
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 42 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

seq_t a(idx_t n) {
  uint64_t m = 2 * (uint64_t)n + 1;
  seq_t pos = 0, neg = 0;
  long long S = 0;
  for (uint64_t d = 1; d <= m; d++) {
    if (m % d != 0) continue;
    uint64_t e = (m / d - 1) / 2;
    uint64_t c = totient_u64(d);
    int sg = (d % 8 == 1 || d % 8 == 7) ? 1 : -1;
    /* Q = floor(2^e / m), R = 2^e mod m */
    seq_t Q = 0;
    uint64_t R = 1;
    for (uint64_t i = 0; i < e; i++) {
      R *= 2;
      Q += Q;
      if (R >= m) {
        R -= m;
        Q += 1;
      }
    }
    seq_t t = Q * (seq_t)c;
    if (sg > 0) {
      pos += t;
      S += (long long)(c * R);
    } else {
      neg += t;
      S -= (long long)(c * R);
    }
  }
  long long q = S / (long long)m;
  seq_t res = pos;
  if (q >= 0) res += (seq_t)(uint64_t)q;
  res -= neg;
  if (q < 0) res -= (seq_t)(uint64_t)(-q);
  res -= 2;
  return res / 2;
}
