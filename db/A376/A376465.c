/*
 * A376465  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *          k)^2*A108625(n-1, k).
 * https://oeis.org/A376465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *            k)^2*A108625(n-1, k), where A108625(r, c) = Sum_{i=0..c}
 *            binomial(r,i)^2*binomial(r+c-i, c-i); a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t r = n - 1;
    size_t m = (size_t)n + 1;

    seq_t *Cr = mem_new(seq_t, m);   /* binomial(r, i) */
    seq_t *D  = mem_new(seq_t, m);   /* binomial(r + j, j) */
    seq_t *A  = mem_new(seq_t, m);   /* binomial(n, k) */
    seq_t *B  = mem_new(seq_t, m);   /* binomial(n + k, k) */

    Cr[0] = 1; D[0] = 1; A[0] = 1; B[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        Cr[i] = (i <= r) ? Cr[i - 1] * (seq_t)(r - i + 1) / (seq_t)i : (seq_t)0;
        D[i]  = D[i - 1] * (seq_t)(r + i) / (seq_t)i;
        A[i]  = A[i - 1] * (seq_t)(n - i + 1) / (seq_t)i;
        B[i]  = B[i - 1] * (seq_t)(n + i) / (seq_t)i;
    }

    seq_t result = 0;
    for (idx_t k = 0; k <= n; k++) {
        seq_t T = 0;
        for (idx_t i = 0; i <= k; i++) {
            T += Cr[i] * Cr[i] * D[k - i];
        }
        seq_t w = A[k] * B[k];
        result += w * w * T;
    }

    mem_free(Cr);
    mem_free(D);
    mem_free(A);
    mem_free(B);
    return result;
}
