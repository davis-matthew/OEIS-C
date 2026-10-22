/*
 * A337524  a(n) = d(n) * (d(n) - 1), where d is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A337524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n)*(d(n)-1)
 * Range:     u8 n <= 179, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for n <= 2^64-1. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0;
    uint64_t bit = 1ULL << 62;      /* largest power of four <= n */
    while (bit > n)
        bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n < 1)                     /* offset is 1; return 0 for n < 1 */
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t s = isqrt_u64(nn);
    uint64_t d_cnt = 0;

    for (uint64_t i = 1; i <= s; ++i) {
        if (nn % i == 0) {
            ++d_cnt;
            if (i != nn / i)
                ++d_cnt;
        }
    }

    /* Product fits in 64 bits for the values considered. */
    return (seq_t)(d_cnt * (d_cnt - 1));
}
