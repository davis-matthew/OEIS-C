/*
 * A365708  a(n) for n >= 2 is the smallest term of A008578 m dividing n such
 *          that n/m is not a square, a(1) = 2.
 * https://oeis.org/A365708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not a perfect square; a(n) = least prime factor
 *            of n if n is a perfect square; a(1) = 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/lpf_simple.h"

/* Integer square root (floor) for 64‑bit unsigned integers */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;           /* highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
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
    if (n == 1)
        return (seq_t)2;

    uint64_t nn = (uint64_t)n;
    uint64_t r = isqrt_uint64(nn);

    if (r * r != nn)
        return (seq_t)1;   /* n is not a perfect square */

    uint64_t p = lpf_simple(nn);
    return (seq_t)p;       /* least prime divisor of n */
}
