/*
 * A331303  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = min(n, A263273(n)), and A263273 is
 *          bijective base-3 reverse.
 * https://oeis.org/A331303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k in [0, f(n)] with k <= A263273(k), where f(n)
 *            = min(n, A263273(n)); this is the restricted growth transform of
 *            A331173, computed by direct counting
 * Range:     u8 n <= 350, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 109 terms through n = 9278 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A331303_rev(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t p = 1;
    while (n % 3 == 0) {
        n /= 3;
        p *= 3;
    }
    idx_t r = 0;
    while (n > 0) {
        r = r * 3 + n % 3;
        n /= 3;
    }
    return r * p;
}

seq_t a(idx_t n)
{
    idx_t r = A331303_rev(n);
    idx_t m0 = (r < n) ? r : n;
    idx_t cnt = 0;
    for (idx_t k = 0; k <= m0; ++k) {
        if (k <= A331303_rev(k))
            ++cnt;
    }
    return (seq_t)cnt;
}
