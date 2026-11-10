/*
 * A261350  Triangle read by rows T(n,k) which is the mirror of A237591.
 * https://oeis.org/A261350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,L+1-j) where row r has length L = A003056(r) and n is
 *            the j-th entry of row r; T(r,k) = ceil((r+1)/k - (k+1)/2) -
 *            ceil((r+1)/(k+1) - (k+2)/2) (A237591, mirrored)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A261350_ceil_div(int64_t num, int64_t den)
{
    if (num >= 0)
        return (num + den - 1) / den;
    return num / den;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t r = 0, cum = 0, len = 0;
    while (cum < (uint64_t)n) {
        ++r;
        while ((len + 1) * (len + 2) / 2 <= r)
            ++len;
        cum += len;
    }
    uint64_t j = (uint64_t)n - (cum - len);   /* 1-based column in mirrored row */
    uint64_t K = len + 1 - j;
    int64_t N = (int64_t)r;
    int64_t k = (int64_t)K;
    int64_t numA = 2 * (N + 1) - k * (k + 1);
    int64_t A = A261350_ceil_div(numA, 2 * k);
    int64_t numB = 2 * (N + 1) - (k + 1) * (k + 2);
    int64_t B = A261350_ceil_div(numB, 2 * (k + 1));
    return (seq_t)(uint64_t)(A - B);
}
