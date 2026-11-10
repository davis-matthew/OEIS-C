/*
 * A356900  a(n) = P(n, 1/2) where P(n, x) = x^(-n)*Sum_{k=0..n} A241171(n,
 *          k)*x^k.
 * https://oeis.org/A356900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A241171(n,k)*2^(n-k), a(0)=1; where
 *            T(m,1)=1, T(m,k) = k*(2k-1)*T(m-1,k-1) + k^2*T(m-1,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t *t = mem_new(seq_t, (size_t)(n + 2));
    for (idx_t i = 0; i <= n + 1; i++) t[i] = 0;
    t[1] = 1;
    for (idx_t m = 2; m <= n; m++) {
        for (idx_t k = m; k >= 1; k--) {
            seq_t kk = (seq_t)k;
            t[k] = kk * (2 * kk - 1) * t[k - 1] + kk * kk * t[k];
        }
    }
    seq_t s = 0;
    for (idx_t k = 1; k <= n; k++) {
        s = s * 2 + t[k];
    }
    mem_free(t);
    return s;
}
