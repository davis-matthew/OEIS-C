/*
 * A327617  a(n) is the least integer k such that digit_sum(A007494(n)*k) =
 *          digit_sum(A007494(n)+k).
 * https://oeis.org/A327617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) is the least k such that digit_sum(A007494(n)*k) =
 *            digit_sum(A007494(n)+k), where A007494(n) = ceil(3*n/2)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute digit sum of an unsigned 128‑bit integer (works for any size). */
static unsigned int digit_sum_uint128(__uint128_t v)
{
    unsigned int sum = 0;
    while (v) {
        sum += (unsigned int)(v % 10u);
        v /= 10u;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* t = ceil(3*n/2) = (3*n)/2 + (n&1) */
    __uint128_t temp = (__uint128_t)3 * n;
    seq_t t = (seq_t)(temp / 2 + (n & 1));

    for (idx_t k = 0; ; ++k) {
        __uint128_t prod = (__uint128_t)t * k;
        __uint128_t sum  = (__uint128_t)t + k;

        if (digit_sum_uint128(prod) == digit_sum_uint128(sum))
            return (seq_t)k;
    }
}
