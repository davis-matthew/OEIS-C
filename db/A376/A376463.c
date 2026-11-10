/*
 * A376463  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *          k)^2*A108625(n, k).
 * https://oeis.org/A376463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2*binomial(n+k,k)^2*T(n,k),
 *            where T(n,k) = A108625(n,k) = Sum_{i=0..k}
 *            binomial(n,i)^2*binomial(n+k-i,k-i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t cnt = (size_t)n + 1;
    seq_t *B = mem_new(seq_t, cnt);   /* B[i] = C(n,i) */
    seq_t *D = mem_new(seq_t, cnt);   /* D[m] = C(n+m,m) */
    B[0] = 1;
    D[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        B[i] = B[i - 1] * (seq_t)(n - i + 1) / (seq_t)i;
        D[i] = D[i - 1] * (seq_t)(n + i) / (seq_t)i;
    }
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        seq_t T = 0;
        for (idx_t i = 0; i <= k; i++) {
            T += B[i] * B[i] * D[k - i];
        }
        seq_t w = B[k] * D[k];
        result += w * w * T;
    }
    mem_free(B);
    mem_free(D);
    return result;
}
