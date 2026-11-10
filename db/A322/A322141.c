/*
 * A322141  a(n) is the sum of the even-indexed terms of the n-th row of the
 *          triangle A237591.
 * https://oeis.org/A322141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k even, k>=2} (P(k) - P(k+1)), where P(k) = floor((n
 *            - k(k-1)/2)/k) if k(k+1)/2 <= n, else 0 (equivalently a(n) = n -
 *            A240542(n))
 * Range:     u8 n <= 829, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9917 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A322141_P(uint64_t n, uint64_t k)
{
    uint64_t t = k * (k + 1) / 2;
    if (t > n) return 0;
    return (n - k * (k - 1) / 2) / k;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t sum = 0;
    for (uint64_t k = 2; k * (k + 1) / 2 <= N; k += 2) {
        sum += A322141_P(N, k) - A322141_P(N, k + 1);
    }
    return (seq_t)sum;
}
