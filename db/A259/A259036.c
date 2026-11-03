/*
 * A259036  Smallest divisor of n^2+1 >= sqrt(n^2+1).
 * https://oeis.org/A259036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = the smallest divisor of n^2 + 1 that is greater than or
 *            equal to sqrt(n^2 + 1) (OEIS A259036). The routine factors n^2+1
 *            by trial division up to its integer square root. The largest
 *            factor <= sqrt(...) is kept; the answer is the complementary
 *            factor. If no factor is found the number is pri...
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9051 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n * (seq_t)n + 1;
    if (N == 1) return 1;
    seq_t sqrtN = floor_sqrt(N);
    seq_t last = 0;
    for (seq_t d = 2; d <= sqrtN; d = d + 1) {
        if ((N % d) == 0) last = d;
    }
    if (last == 0) return N;       /* N is prime */
    return N / last;
}
