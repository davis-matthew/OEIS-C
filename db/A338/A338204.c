/*
 * A338204  a(n) is the sum of odd-indexed terms (of every row) of the first n
 *          rows of the triangle A237591.
 * https://oeis.org/A338204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=1..n} Sum_{k odd, 1<=k<=A003056(r)} T(r,k), where
 *            T(r,k) = f(r,k) - f(r,k+1) and f(r,k) = ceil((r+1)/k - (k+1)/2)
 *            (A237591)
 * Range:     u8 n <= 26, u16 n <= 434, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 76 terms through n = 4626 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A338204_f(int64_t r, int64_t k)
{
    int64_t num = 2 * (r + 1) - k * (k + 1);
    int64_t den = 2 * k;
    if (num >= 0)
        return (num + den - 1) / den;
    return num / den;
}

seq_t a(idx_t n)
{
    int64_t total = 0;
    int64_t N = (int64_t)n;
    for (int64_t r = 1; r <= N; ++r) {
        for (int64_t k = 1; k * (k + 1) / 2 <= r; k += 2) {
            total += A338204_f(r, k) - A338204_f(r, k + 1);
        }
    }
    return (seq_t)total;
}
