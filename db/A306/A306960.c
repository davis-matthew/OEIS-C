/*
 * A306960  Trajectory of 1 under repeated application of x -> A306958(x).
 * https://oeis.org/A306960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1, a(n) = A306958(a(n-1)), where A306958(m) = Sum_{digits
 *            d of m} d!*binomial(10,d); the trajectory ends in a cycle of
 *            length 4 (iteration with cycle reduction)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A306958_step(uint64_t m)
{
    static const uint64_t contrib[10] = {
        1ULL, 10ULL, 90ULL, 720ULL, 5040ULL, 30240ULL,
        151200ULL, 604800ULL, 1814400ULL, 3628800ULL
    };
    uint64_t r = 0;
    if (m == 0) return contrib[0];
    while (m > 0) {
        r += contrib[m % 10];
        m /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t steps = (uint64_t)(n - 1);
    uint64_t x = 1;
    if (steps > 200) steps = 200 + (steps - 200) % 4;
    while (steps > 0) {
        x = A306958_step(x);
        steps--;
    }
    return (seq_t)x;
}
