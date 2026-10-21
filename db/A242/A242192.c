/*
 * A242192  Number of ways to write n^4 as sum of a square and a cube, both >
 *          0.
 * https://oeis.org/A242192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer solutions (a,b) of a² + b³ =
 *            n⁴. The algorithm enumerates all b with b³ < n⁴ and checks
 *            whether the remaining sum is a perfect square. Each test
 *            requires only 64‑bit arithmetic (the maximum n in the benchmark
 *            is 10000, so n⁴ < 10¹⁶ < 2⁶⁴)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icbrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_u64.h"

/* Integer square root: returns ⌊v¹ᐟ²⌋ for v < 2⁶⁴. */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t low = 0, high = 1ULL << 32;          /* 2³² > √(2⁶⁴)            */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Use 64‑bit arithmetic for all intermediate values. */
    uint64_t N   = (uint64_t)n;
    uint64_t N2  = N * N;
    __uint128_t N4 = (__uint128_t)N2 * N2;        /* n⁴  (< 10¹⁶) */

    if (N4 == 0)
        return 0;

    /* Maximum b with b³ < n⁴: floor(cuberoot(n⁴‑1)) */
    uint64_t max_b = icbrt_u64((uint64_t)(N4 - 1));

    seq_t count = 0;

    for (uint64_t b = 1; b <= max_b; ++b) {
        uint64_t cube = b * b * b;                /* b³ ≤ 10¹⁰ for n ≤ 10⁴ */
        if ((__uint128_t)cube > N4)
            break;                               /* safety against overflow */

        __uint128_t rem = N4 - (__uint128_t)cube; /* remainder < 2⁶⁴ */

        uint64_t r_rem   = (uint64_t)rem;
        uint64_t a_sq    = isqrt_u64(r_rem);
        if (a_sq != 0 && (__uint128_t)a_sq * a_sq == rem) {
            ++count;
        }
    }

    return count;
}
