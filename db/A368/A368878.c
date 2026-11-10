/*
 * A368878  a(n) is the least k such that A368877^k(n) < n or -1 if no such k
 *          exists.
 * https://oeis.org/A368878
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>=1 such that g^k(n) < n, where g = A368877, g(m)
 *            = f^bitlength(m)(m), f(m) = m/2 if even, (3m+1)/2 if odd; -1 if
 *            none (found by iteration, capped)
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9307 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A368877_a(seq_t x)
{
    unsigned k = 0;
    seq_t t = x;
    while (t > 0) { t >>= 1; k++; }
    for (unsigned i = 0; i < k; ++i) {
        if ((x & 1) == 0) x >>= 1;
        else x = (x * 3 + 1) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t x = m;
    for (long long k = 1; k <= 200000; ++k) {
        x = A368877_a(x);
        if (x < m) return (seq_t)k;
        if (x <= 2) break; /* cycle at 2 (2 -> 1 -> 2) cannot fall below m unless it did already */
    }
    return (seq_t)-1;
}
