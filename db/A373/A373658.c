/*
 * A373658  a(n) = Sum_{k=0..n} binomial(n, k)*A050446(n, n - k). Alternating
 *          row sums of A373660.
 * https://oeis.org/A373658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*T(n,n-k), where T(n,m) =
 *            A050446 as a table: T(0,m) = 1, T(n,0) = 1, T(n,m) = T(n,m-1) +
 *            Sum_{j=0..floor((n-1)/2)} T(2j,m-1)*T(n-1-2j,m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n + 1;
    seq_t *T = mem_new(seq_t, N * N);
    seq_t *row = mem_new(seq_t, N + 1);
    size_t i, j, k, m;

    for (i = 0; i < N; i++) T[i * N] = 1;
    for (j = 0; j < N; j++) T[j] = 1;

    for (m = 1; m < N; m++) {
        for (i = 1; i < N; i++) {
            seq_t val = T[i * N + (m - 1)];
            size_t maxk = (i - 1) / 2;
            for (k = 0; k <= maxk; k++)
                val += T[(2 * k) * N + (m - 1)] * T[(i - 1 - 2 * k) * N + m];
            T[i * N + m] = val;
        }
    }

    /* Pascal row n */
    row[0] = 1;
    for (i = 1; i <= (size_t)n; i++) {
        row[i] = 1;
        for (j = i - 1; j >= 1; j--) row[j] = row[j] + row[j - 1];
    }

    seq_t sum = 0;
    for (k = 0; k <= (size_t)n; k++)
        sum += row[k] * T[(size_t)n * N + ((size_t)n - k)];

    mem_free(T);
    mem_free(row);
    return sum;
}
