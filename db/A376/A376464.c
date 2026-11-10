/*
 * A376464  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *          k)^2*A108625(n, n-k).
 * https://oeis.org/A376464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^2*C(n+k,k)^2*T(n,n-k), where T(r,c) =
 *            A108625 array entry = Sum_{i=0..c} C(r,i)^2*C(r+c-i,c-i)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t m = (size_t)n + 1;
    seq_t *B = mem_new(seq_t, m);   /* B[i] = C(n,i) */
    seq_t *D = mem_new(seq_t, m);   /* D[j] = C(n+j,j) */
    B[0] = 1;
    D[0] = 1;
    for (idx_t i = 0; i < n; i++) {
        B[i + 1] = B[i] * (seq_t)(n - i) / (seq_t)(i + 1);
        D[i + 1] = D[i] * (seq_t)(n + i + 1) / (seq_t)(i + 1);
    }
    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        idx_t c = n - k;
        seq_t T = 0;
        for (idx_t i = 0; i <= c; i++) {
            T += B[i] * B[i] * D[c - i];
        }
        result += B[k] * B[k] * D[k] * D[k] * T;
    }
    mem_free(B);
    mem_free(D);
    return result;
}
