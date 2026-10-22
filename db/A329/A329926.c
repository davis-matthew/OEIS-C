/*
 * A329926  Beatty sequence for (9+sqrt(41))/5.
 * https://oeis.org/A329926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (9 + sqrt(41)) / 5) (Beatty sequence for
 *            (9+sqrt(41))/5)
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static uint64_t isqrt_u128(__uint128_t x) {
    if (x == 0) return 0;
    __uint128_t r = x;
    while (1) {
        __uint128_t r_next = (r + x / r) >> 1;
        if (r_next >= r) break;
        r = r_next;
    }
    /* Ensure r is the floor of the square root */
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return (uint64_t)r;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1.
       Computation uses only integer arithmetic. */
    if (n == 0) return 0; /* fallback, not part of the OEIS definition */

    /* Compute floor(n * sqrt(41)) as floor(sqrt(41*n*n)) */
    __uint128_t nsq = (__uint128_t)n * n;      /* n^2 */
    __uint128_t m   = nsq * 41;               /* 41 * n^2 */
    uint64_t  sroot = isqrt_u128(m);          /* floor(sqrt(41*n^2)) = floor(n*sqrt(41)) */

    /* Numerator of the Beatty expression: 9*n + floor(n*sqrt(41)) */
    seq_t numerator = (seq_t)9 * n + (seq_t)sroot;
    seq_t result    = numerator / 5;          /* floor division by 5 */

    return result;
}
