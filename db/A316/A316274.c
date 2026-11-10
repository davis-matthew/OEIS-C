/*
 * A316274  Nonzero terms in row sums of the lower triangular part of a square
 *          matrix formed by Dirichlet convolution of adjacent columns in the
 *          square matrix A191898.
 * https://oeis.org/A316274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A298825(A001694(n)); with m = A001694(n), a(n) =
 *            Sum_{k=1..m} Sum_{d|m} T(d,k)*T(m/d,k+1), where T is the
 *            symmetric array A191898
 * Range:     i8 n <= 10, i16 n <= 37, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-08)
 * Helpers:   is_squarefree_u64, isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_squarefree_u64.h"
#include "../utils/isqrt_u64.h"

static uint64_t p316274_count(uint64_t x) {
    uint64_t total = 0, b;
    for (b = 1; b * b * b <= x; b++)
        if (is_squarefree_u64(b))
            total += isqrt_u64(x / (b * b * b));
    return total;
}

static uint64_t p316274_powerful(uint64_t n) {
    uint64_t lo = 1, hi = n * n;
    if (hi < 1) hi = 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (p316274_count(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    long long m = (long long)p316274_powerful((uint64_t)n);
    long long N = m + 1;
    long long stride = N + 1;
    seq_t *T = mem_new(seq_t, (size_t)(stride * stride));
    long long r, k, d;

    for (r = 1; r <= N; ++r) {
        T[r * stride + 1] = (seq_t)1;
        T[1 * stride + r] = (seq_t)1;
    }
    for (k = 2; k <= N; ++k) {
        seq_t W = (seq_t)0;
        for (r = 1; r < k; ++r) W += T[r * stride + k];
        for (r = k; r <= N; ++r) {
            seq_t v = -W;
            T[r * stride + k] = v;
            T[k * stride + r] = v;
            W += v - T[(r - k + 1) * stride + k];
        }
    }

    seq_t total = (seq_t)0;
    for (k = 1; k <= m; ++k) {
        for (d = 1; d <= m; ++d) {
            if (m % d == 0)
                total += T[d * stride + k] * T[(m / d) * stride + (k + 1)];
        }
    }
    mem_free(T);
    return total;
}
