/*
 * A376466  a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *          k)^2*A108625(n-1, k).
 * https://oeis.org/A376466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *            k)^2*A108625(n-1, k), where A108625(r, c) = Sum_{i=0..c}
 *            binomial(r,i)^2*binomial(r+c-i, c-i); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    size_t N = (size_t)n;
    size_t W = 2 * N + 1;
    seq_t *P = mem_new(seq_t, W * W);
    size_t i, j;
    for (i = 0; i < W; i++) {
        P[i * W] = 1;
        for (j = 1; j <= i; j++)
            P[i * W + j] = P[(i - 1) * W + j - 1] + (j <= i - 1 ? P[(i - 1) * W + j] : (seq_t)0);
    }
    size_t r = N - 1;
    seq_t total = 0;
    for (size_t k = 0; k <= N; k++) {
        seq_t T = 0;
        for (i = 0; i <= k && i <= r; i++) {
            seq_t c1 = P[r * W + i];
            T += c1 * c1 * P[(r + k - i) * W + (k - i)];
        }
        seq_t c2 = P[(N + k) * W + k];
        seq_t term = P[N * W + k] * c2 * c2 * T;
        if (((N + k) & 1) != 0) total -= term;
        else total += term;
    }
    mem_free(P);
    return total;
}
