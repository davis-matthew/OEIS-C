/*
 * A256971  Partial sums of A256970.
 * https://oeis.org/A256971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A256970(k), where A256970(k) = smallest
 *            prime divisor of 4*k^2+1 (trial division by p == 1 mod 4)
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 2646, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9290 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A256970_small(uint64_t k)
{
    uint64_t val = 4 * k * k + 1;
    for (uint64_t p = 5; p <= val / p; p += 4)
        if (val % p == 0) return p;
    return val;
}

static seq_t A256970_big(seq_t k)
{
    seq_t val = 4 * k * k + 1;
    for (seq_t p = 5; p <= val / p; p += 4)
        if (val % p == 0) return p;
    return val;
}

static seq_t A256970_a(idx_t k)
{
    if (k < ((idx_t)1 << 31))
        return (seq_t)A256970_small((uint64_t)k);
    return A256970_big((seq_t)k);
}

seq_t a(idx_t n)
{
    static idx_t cached_n = 0;
    static seq_t cached_sum = 0;
    if (n < cached_n) {
        cached_n = 0;
        cached_sum = 0;
    }
    seq_t s = cached_sum;
    for (idx_t k = cached_n + 1; k <= n; k++)
        s += A256970_a(k);
    cached_n = n;
    cached_sum = s;
    return s;
}
