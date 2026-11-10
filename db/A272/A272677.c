/*
 * A272677  Number of digits in A018796 - number of digits in n.
 * https://oeis.org/A272677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of digits of A018796(n) minus digits of n;
 *            computed by search: a(n) = least k >= 0 such that [n*10^k,
 *            (n+1)*10^k - 1] contains a square (a(0) = 0)
 * Range:     u8 n <= 4, u16 n <= 1570, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 8083 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A272677_isqrt(seq_t x)
{
    seq_t res = 0, bit = 1;
    while (bit <= x / 4) bit <<= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t p = 1;
    for (seq_t k = 0; ; k++) {
        seq_t low = N * p;
        seq_t high = (N + 1) * p - 1;
        seq_t r = A272677_isqrt(high);
        if (r * r >= low) return k;
        p *= 10;
    }
}
