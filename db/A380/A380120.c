/*
 * A380120  Triangle read by rows: T(n, k) is the number of walks of length n
 *          on the Z X Z grid with unit steps in all four directions (NSWE)
 *          starting at (0, 0). k is the absolute value of the x-coordinate of
 *          the endpoint of the walk.
 * https://oeis.org/A380120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   a(n) = T(m,k) where m = floor((sqrt(8*n + 1) - 1) / 2), k = n -
 *            m*(m+1)/2. T(m,k) = binomial(2*m, m-k) if k == 0, 2 *
 *            binomial(2*m, m-k) otherwise
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static inline idx_t isqrt_u128(__uint128_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)0xFFFFFFFFFFFFFFFFULL;   /* maximum idx_t value */

    while (low < high) {
        idx_t mid   = low + ((high - low) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* m = floor((sqrt(8*n + 1) - 1) / 2) */
    __uint128_t val = (( __uint128_t)n << 3) + 1;          /* 8*n + 1  */
    idx_t s = isqrt_u128(val);
    idx_t m = (s - 1) >> 1;                               /* floor((s-1)/2) */

    /* k = n - m*(m+1)/2 */
    idx_t triangular = (idx_t)(( (__uint128_t)m * (m + 1) ) >> 1); /* m*(m+1)/2 */
    idx_t k = n - triangular;

    idx_t two_m = m << 1;
    seq_t result;

    if (k == 0) {
        result = binomial(two_m, m);
    } else {
        result = binomial(two_m, m - k);
        result = mul_small(result, 2);                     /* multiply by 2 */
    }
    return result;
}
