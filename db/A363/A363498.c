/*
 * A363498  a(n) = Sum_{k=0..n} floor(sqrt(k))^4.
 * https://oeis.org/A363498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*⌊√n⌋⁴ - (1/30)*⌊√n⌋*(⌊√n⌋+1)*(20*⌊√n⌋⁴ + 4*⌊√n⌋³ -
 *            14*⌊√n⌋² + 4*⌊√n⌋ + 1)
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 2382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root: floor(sqrt(n)) for 0 ≤ n < 2⁶⁴. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* high is exclusive */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t midsq = (__uint128_t)mid * mid;
        if (midsq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Sequence function for A363498. */
seq_t a(idx_t n)
{
    idx_t m     = isqrt_u64(n);          /* ⌊√n⌋ */
    __uint128_t m128 = m;
    __uint128_t m2   = m128 * m128;      /* m² */
    __uint128_t m4   = m2 * m2;          /* m⁴ */
    __uint128_t m3   = m128 * m2;        /* m³ */

    /* term = 20*m⁴ + 4*m³ - 14*m² + 4*m + 1 */
    __uint128_t term = (__uint128_t)20 * m4
                     + (__uint128_t)4  * m3
                     - (__uint128_t)14 * m2
                     + (__uint128_t)4  * m128
                     + (__uint128_t)1;

    /* prod = m*(m+1)*term / 30 */
    __uint128_t prod = m128 * (m128 + 1) * term / (__uint128_t)30;

    /* result = (n+1)*m⁴ - prod */
    __uint128_t res128 = (__uint128_t)(n + 1) * m4 - prod;

    return (seq_t)res128;
}
