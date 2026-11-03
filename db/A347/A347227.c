/*
 * A347227  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{d|n} mu(d)*mu(n/d)*d^k.
 * https://oeis.org/A347227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 70, i32 n <= 158, i64 n <= 415,
 *            i128 n <= 1138, big any n
 * Validated: standard, pass, 82 terms through n = 9864 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347227: the square array T(n,k) = Sum_{d|n} mu(d) mu(n/d) d^k, n >= 1,
 * k >= 0, read by antidiagonals downwards (n + k constant, n rising). Ring
 * operations only, so exact modulo the width. */

#include <stdint.h>

static int mu(uint64_t m)
{
    int r = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p) continue;
        m /= p;
        if (m % p == 0) return 0;
        r = -r;
    }
    return m > 1 ? -r : r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t s = 0, i = (uint64_t)n - 1;              /* antidiagonal s has s + 1 entries */
    while (i > s) { i -= s + 1; ++s; }
    const uint64_t N = i + 1, K = s - i;              /* downwards: n = 1 .. s+1 */
    seq_t sum = (seq_t)0;
    for (uint64_t d = 1; d <= N; ++d) {
        if (N % d) continue;
        int w = mu(d) * mu(N / d);
        if (!w) continue;
        seq_t p = (seq_t)1;
        for (uint64_t j = 0; j < K; ++j) p *= (seq_t)d;
        if (w > 0) sum += p; else sum -= p;
    }
    return sum;
}
