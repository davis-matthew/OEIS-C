/*
 * A269938  Row sums of A268434.
 * https://oeis.org/A269938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), T(0,0)=1, T(n,k) = T(n-1,k-1) +
 *            ((n-1)^2+k^2)*T(n-1,k), T(n,k)=0 for k<0 or k>n (A268434)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *t = mem_new(seq_t, N + 2);
    t[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        seq_t base = (seq_t)(m - 1) * (seq_t)(m - 1);
        for (size_t k = m + 1; k-- > 0; ) {
            seq_t prev = (k > 0) ? t[k - 1] : (seq_t)0;
            t[k] = prev + (base + (seq_t)k * (seq_t)k) * t[k];
        }
    }
    seq_t s = 0;
    for (size_t k = 0; k <= N; ++k) s += t[k];
    mem_free(t);
    return s;
}
