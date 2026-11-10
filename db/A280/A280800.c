/*
 * A280800  a(n) = A049501(cototient(n)).
 * https://oeis.org/A280800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049501(A051953(n)), where A051953(n) = n - phi(n) and
 *            A049501(m) = sum of positions (leftmost bit = 1) of 1-bits
 *            followed by a 0 in binary m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9450 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A280800_phi(uint64_t m)
{
    uint64_t result = m;
    uint64_t d;
    for (d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / d;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

static uint64_t A280800_major(uint64_t n)
{
    if (n == 0) return 0;
    int msb = 0;
    uint64_t tmp = n;
    while (tmp >>= 1) msb++;
    msb++;
    uint64_t sum = 0;
    int i;
    for (i = msb - 1; i > 0; --i) {
        if (((n >> i) & 1ULL) && !((n >> (i - 1)) & 1ULL)) {
            sum += (uint64_t)(msb - i);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t c = (uint64_t)n - A280800_phi((uint64_t)n);
    return (seq_t)A280800_major(c);
}
