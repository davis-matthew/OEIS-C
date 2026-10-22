/*
 * A396622  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=sqrt(3).
 * https://oeis.org/A396622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where r(1,k)=floor(k*sqrt(3)), r(n,k)=f(r(n-1,k)),
 *            f(x)=floor(x*sqrt(3))
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t sqrt_uint128(__uint128_t x)
{
    if (x == 0) return 0;
    __uint128_t low = 0, high = x;
    while (low <= high) {
        __uint128_t mid = (low + high) >> 1;
        if (mid == 0) { low = 1; continue; }
        /* avoid overflow by division test */
        __uint128_t div = x / mid;
        if (mid <= div) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high; /* floor(sqrt(x)) */
}

static inline seq_t f(seq_t x)
{
    /* f(x) = floor(x * sqrt(3)) = floor( sqrt(3)*x ) */
    /* compute as floor(sqrt(3*x*x)) to avoid floating point */
    __uint128_t prod = (__uint128_t)3 * (__uint128_t)x * (__uint128_t)x;
    return (seq_t)sqrt_uint128(prod);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t value = (seq_t)n;
    for (idx_t i = 0; i < n; ++i) {
        value = f(value);
    }
    return value;
}
