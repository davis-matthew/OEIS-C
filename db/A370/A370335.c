/*
 * A370335  Expansion of Sum_{n>=0} 5^n * (2*4^n + 1)/3 * x^(n*(n+1)/2).
 * https://oeis.org/A370335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n is a triangular number T_k = k*(k+1)/2 then 5^k *
 *            (2*4^k + 1)/3, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 35, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer sqrt for unsigned __int128 values.
   Returns the largest x such that x*x <= v. */
static idx_t isqrt_uint128(__uint128_t v)
{
    idx_t lo = 0, hi = ((idx_t)1 << 63) - 1;  /* hi fits in idx_t */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n == 0 gives a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Compute discriminant d = 1 + 8*n, check if it is a perfect square */
    __uint128_t d = (__uint128_t)1 + (__uint128_t)8 * n;
    idx_t sqrt_d = isqrt_uint128(d);
    if ((__uint128_t)sqrt_d * sqrt_d != d)
        return (seq_t)0;                      /* n not triangular */

    /* k = (sqrt_d - 1) / 2  should satisfy k*(k+1)/2 == n */
    idx_t k = (sqrt_d - 1) / 2;
    if ((__uint128_t)k * (k + 1) / 2 != n)
        return (seq_t)0;                      /* safety check */

    /* Compute 4^k and 5^k using simple repeated multiplication */
    seq_t pow4 = (seq_t)1;
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
    {
        pow4 = pow4 * (seq_t)4;
        pow5 = pow5 * (seq_t)5;
    }

    /* numerator = 2*4^k + 1, guaranteed divisible by 3 */
    seq_t numerator = (seq_t)2 * pow4 + (seq_t)1;

    /* final result */
    return pow5 * (numerator / (seq_t)3);
}
