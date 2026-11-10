/*
 * A364492  a(n) = A163511(n) / gcd(n, A163511(n)).
 * https://oeis.org/A364492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A163511(n) / gcd(n, A163511(n)); A163511(0)=1, A163511(n)
 *            = p(r) * Product_{i=1..r} p(i)^(pos_i - pos_{i-1} - 1), where r
 *            = A000120(n) and pos_i is the 1-based position of the i-th set
 *            bit of n (counting from the least significant bit)
 * Range:     u8 n <= 34, u16 n <= 258, u32 n <= 8206, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 9691 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A364492_primes[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

static seq_t A163511_a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t pos[64];
    idx_t cnt = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }
    seq_t result = (seq_t)A364492_primes[cnt - 1];
    idx_t prev = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        idx_t curr = pos[i];
        idx_t ex = curr - prev - 1;
        for (idx_t e = 0; e < ex; ++e) {
            result *= (seq_t)A364492_primes[i];
        }
        prev = curr;
    }
    return result;
}

static seq_t A364492_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t A = A163511_a(n);
    seq_t g = A364492_gcd((seq_t)n, A);
    return A / g;
}
