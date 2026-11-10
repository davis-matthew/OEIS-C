/*
 * A325562  Number of iterations of A234741 needed when started from n before
 *          a fixed point is reached.
 * https://oeis.org/A325562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if A234741(n) = n, otherwise a(n) = 1 + a(A234741(n));
 *            computed by direct iteration
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9950 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A325562_clmul(uint64_t x, uint64_t y)
{
    uint64_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static uint64_t A325562_A234741(uint64_t n)
{
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            result = A325562_clmul(result, p);
            m /= p;
        }
    }
    if (m > 1) result = A325562_clmul(result, m);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t count = 0;
    if (x == 0) return 0;
    for (;;) {
        uint64_t y = A325562_A234741(x);
        if (y == x) break;
        x = y;
        ++count;
    }
    return (seq_t)count;
}
