/*
 * A306962  Trajectory of 4 under repeated application of x -> A306958(x).
 * https://oeis.org/A306962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 4; a(n) = A306958(a(n-1)), where A306958(m) = Sum_{digits
 *            d of m} d!*binomial(10,d)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A306962_step(seq_t m)
{
    static const unsigned long long contrib[10] = {
        1ULL, 10ULL, 90ULL, 720ULL, 5040ULL, 30240ULL,
        151200ULL, 604800ULL, 1814400ULL, 3628800ULL
    };
    seq_t result = 0;
    if (m == 0)
        return (seq_t)contrib[0];
    while (m > 0) {
        int d = (int)(m % 10);
        result += (seq_t)contrib[d];
        m /= 10;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = 4;
    idx_t i;
    for (i = 1; i < n; i++)
        x = A306962_step(x);
    return x;
}
