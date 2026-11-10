/*
 * A280062  a(n) = A049502(A000142(n)).
 * https://oeis.org/A280062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049502(n!) = sum of (p+1) over bit positions p (0-based)
 *            where bit p of n! is 1 and bit p+1 is 0. n! is built as a native
 *            multi-limb integer
 * Range:     u8 n <= 17, u16 n <= 129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9941 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A280062_bit(const uint32_t *d, unsigned long long p)
{
    return (int)((d[p >> 5] >> (p & 31)) & 1u);
}

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long nb = 0, t = nn;
    while (t) { nb++; t >>= 1; }
    size_t cap = (size_t)((nn * (nb + 1)) / 32 + 4);
    uint32_t *d = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t cnt = 1;
    d[0] = 1;
    for (unsigned long long i = 2; i <= nn; i++) {
        uint64_t carry = 0;
        for (size_t k = 0; k < cnt; k++) {
            uint64_t cur = (uint64_t)d[k] * i + carry;
            d[k] = (uint32_t)cur;
            carry = cur >> 32;
        }
        while (carry) {
            d[cnt++] = (uint32_t)carry;
            carry >>= 32;
        }
    }
    unsigned long long top = 0;
    uint32_t tw = d[cnt - 1];
    while (tw) { top++; tw >>= 1; }
    unsigned long long len = (unsigned long long)(cnt - 1) * 32 + top;
    unsigned long long sum = 0;
    for (unsigned long long p = 0; p + 1 < len; p++) {
        if (A280062_bit(d, p) && !A280062_bit(d, p + 1))
            sum += p + 1;
    }
    free(d);
    return (seq_t)sum;
}
