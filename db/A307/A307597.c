/*
 * A307597  Number of partitions of n into 2 distinct positive triangular
 *          numbers.
 * https://oeis.org/A307597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered pairs of distinct positive triangular
 *            numbers whose sum is n. Triangular numbers are T_k = k*(k+1)/2
 *            for k >= 1
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 125 terms through n = 9974 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307597: the number of partitions of n into 2 distinct positive triangular
 * numbers: the i with T_i < n - T_i and n - T_i triangular (8m + 1 a square).
 * Computed in 64 bits for any n (the original kept 200 triangular numbers,
 * enough only for n <= 20100). */

#include <stdint.h>

static uint64_t isqrt64(uint64_t m)
{
    uint64_t r = 0;
    for (uint64_t bit = UINT64_C(1) << 62; bit; bit >>= 2) {
        if (m >= r + bit) { m -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    const uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    for (uint64_t i = 1; i * (i + 1) / 2 < N - i * (i + 1) / 2 && i * (i + 1) <= N; ++i) {
        uint64_t m = N - i * (i + 1) / 2;
        uint64_t s = isqrt64(8 * m + 1);
        if (s * s == 8 * m + 1) ++count;         /* m = T_j with j = (s - 1)/2 > i */
    }
    return (seq_t)count;
}
