/*
 * A376460  a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *          k)^2*A108625(n, k).
 * https://oeis.org/A376460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} (-1)^(n+k)*binomial(n, k)*binomial(n+k,
 *            k)^2*A108625(n, k), where A108625(n,k) = Sum_{i=0..k}
 *            binomial(n,i)^2*binomial(n+k-i,k-i)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
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
    size_t N = (size_t)n;
    size_t W = 2 * N + 1;
    seq_t *P = mem_new(seq_t, W * W);
    size_t m, j, k, i;
    seq_t pos = 0, neg = 0;

    /* Pascal's triangle up to row 2n */
    for (m = 0; m < W; m++) {
        P[m * W] = 1;
        for (j = 1; j <= m; j++) {
            seq_t v = P[(m - 1) * W + (j - 1)];
            if (j <= m - 1) v += P[(m - 1) * W + j];
            P[m * W + j] = v;
        }
    }

    for (k = 0; k <= N; k++) {
        seq_t T = 0;
        for (i = 0; i <= k; i++) {
            seq_t b = P[N * W + i];
            T += b * b * P[(N + k - i) * W + (k - i)];
        }
        seq_t c = P[(N + k) * W + k];
        seq_t term = P[N * W + k] * c * c * T;
        if (((N + k) & 1) == 0) pos += term;
        else neg += term;
    }

    mem_free(P);
    return pos - neg;
}
