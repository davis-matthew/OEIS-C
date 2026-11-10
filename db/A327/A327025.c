/*
 * A327025  a(n) = Sum_{k=0..n} k! * Sum_{d|n} phi(d) * A008284(n/d, k).
 * https://oeis.org/A327025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * Sum_{d|n} phi(d) * A008284(n/d, k),
 *            with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static uint64_t A327025_phi(uint64_t m)
{
    uint64_t r = m;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    size_t N = (size_t)n;
    size_t W = N + 1;
    seq_t *P = mem_new(seq_t, W * W);
    P[0] = 1;
    for (size_t m = 1; m <= N; ++m)
        for (size_t k = 1; k <= m; ++k)
            P[m * W + k] = P[(m - 1) * W + (k - 1)] + P[(m - k) * W + k];

    seq_t *F = mem_new(seq_t, W);   /* F[m] = sum_k k! * P(m,k) */
    for (size_t m = 1; m <= N; ++m) {
        seq_t fact = 1, s = 0;
        for (size_t k = 1; k <= m; ++k) {
            fact = fact * (seq_t)k;
            s += fact * P[m * W + k];
        }
        F[m] = s;
    }

    seq_t total = 0;
    for (size_t d = 1; d <= N; ++d) {
        if (N % d == 0)
            total += (seq_t)A327025_phi((uint64_t)d) * F[N / d];
    }
    mem_free(F);
    mem_free(P);
    return total;
}
