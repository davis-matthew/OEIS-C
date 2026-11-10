/*
 * A306959  The unique cycle of length 4 under iteration of the map x ->
 *          A306958(x).
 * https://oeis.org/A306959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(1) = 756822, a(n+1) = A306958(a(n)), where A306958(x) =
 *            Sum_{i} d_i!*binomial(10,d_i) over decimal digits d_i of x; the
 *            cycle has length 4, so n is taken mod 4
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A306958_step(uint64_t x)
{
    static const uint64_t contrib[10] = {
        1ULL, 10ULL, 90ULL, 720ULL, 5040ULL, 30240ULL,
        151200ULL, 604800ULL, 1814400ULL, 3628800ULL
    };
    uint64_t r = 0;
    if (x == 0) return contrib[0];
    while (x > 0) {
        r += contrib[x % 10];
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t x = 756822ULL;
    unsigned k = (unsigned)((n - 1) % 4);
    unsigned i;
    for (i = 0; i < k; i++)
        x = A306958_step(x);
    return (seq_t)x;
}
