/*
 * A329923  Beatty sequence for (2+sqrt(34))/5.
 * https://oeis.org/A329923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2+sqrt(34))/5)
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Integer square root for a 64‑bit unsigned value.
 * Returns floor(sqrt(val)).  Uses binary search and
 * 128‑bit intermediate to avoid overflow.
 */
static uint64_t isqrt_u64(uint64_t val)
{
    uint64_t low = 0;
    uint64_t high = 0xFFFFFFFFULL;        /* sqrt(2^64-1) < 2^32, fits in 64 bits */

    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= val)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*
 * a(n) = floor(n*(2+sqrt(34))/5)
 *
 * Computed as floor((2n + floor(n*sqrt(34)))/5).
 * floor(n*sqrt(34)) is obtained as floor(sqrt(34*n*n)).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* value for n < 1, not used in OEIS offset */

    /* use uint64_t for intermediate arithmetic (n is uint64_t) */
    uint64_t n64   = (uint64_t)n;
    __uint128_t prod = (__uint128_t)n64 * n64 * 34;  /* 34 * n^2  */
    uint64_t prod64 = (uint64_t)prod;                /* fits for n up to 10⁴ */

    /* floor(sqrt(34 * n^2)) */
    uint64_t sq32 = isqrt_u64(prod64);
    seq_t sq   = (seq_t)sq32;
    seq_t nn   = (seq_t)n64;

    seq_t numerator = 2 * nn + sq;
    return numerator / 5;
}
