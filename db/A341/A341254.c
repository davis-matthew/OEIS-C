/*
 * A341254  a(n) = floor(r*floor(r*n)), where r = (2 + sqrt(5))/2.
 * https://oeis.org/A341254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(r * floor(r * n)) where r = (2 + sqrt(5)) / 2
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root of a 128‑bit unsigned integer */
static inline __uint128_t isqrt_uint128(__uint128_t x)
{
    __uint128_t low = 0, high = x;
    while (low <= high) {
        __uint128_t mid = low + ((high - low) >> 1);
        __uint128_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Compute floor(k * sqrt(5) / 2) using only integer arithmetic.
   k is an unsigned integer type seq_t. */
static inline seq_t floor_half_sqrt5_seq(seq_t k)
{
    /* 5 * k * k fits into 128 bits for all seq_t that appear in the test set. */
    __uint128_t kk   = (__uint128_t)k * (__uint128_t)k;
    __uint128_t prod = kk * 5;
    __uint128_t s    = isqrt_uint128(prod);        /* floor(sqrt(5 * k^2)) = floor(k * sqrt(5)) */
    return (seq_t)(s / 2);                        /* floor(k * sqrt(5) / 2) */
}

seq_t a(idx_t n)
{
    seq_t kn = (seq_t)n;

    /* floor(r * n) = n + floor(n * sqrt(5) / 2) */
    seq_t floor_rn = kn + floor_half_sqrt5_seq(kn);

    /* a(n) = floor(r * floor(r * n)) = floor_rn + floor(floor_rn * sqrt(5) / 2) */
    return floor_rn + floor_half_sqrt5_seq(floor_rn);
}
