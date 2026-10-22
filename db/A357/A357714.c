/*
 * A357714  a(n) is the number of equations in the set E_{n,b} :=
 *          {x+2^b*y=n^b, 2^b*x+3^b*y=n^b, ..., k^b*x+(k+1)^b*y=n^b, ...,
 *          n^b*x+(n+1)^b*y=n^b} which admit at least one nonnegative integer
 *          solution when b is sufficiently large.
 * https://oeis.org/A357714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n) - 3/2) + d(n) where d(n) = number of
 *            divisors of n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 32;          /* 2^32 > sqrt(2^64-1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Number of positive divisors of a 64‑bit unsigned integer */
static inline uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t count = 1;
    uint64_t temp  = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (temp > 1)          /* remaining prime factor */
        count *= 2;
    return count;
}

seq_t a(idx_t n)
{
    /* Convert index to 64‑bit unsigned for arithmetic */
    uint64_t m = (uint64_t)n;

    /* Term 1: ceil(sqrt(m) - 3/2) */
    uint64_t s = isqrt_u64(m);
    __uint128_t threshold = (__uint128_t)s * s + s;  /* s^2 + s */
    uint64_t ceil_term = ( (__uint128_t)m <= threshold ) ? (s - 1) : s;

    /* Term 2: number of divisors of m */
    uint64_t divcnt = divisor_count_u64(m);

    /* Result: sum of the two terms */
    return (seq_t)(ceil_term + divcnt);
}
