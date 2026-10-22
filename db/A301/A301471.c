/*
 * A301471  Number of ways to write n^2 as x^2 + 2*y^2 + 3*2^z with x,y,z
 *          nonnegative integers.
 * https://oeis.org/A301471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n^2 as x^2 + 2*y^2 + 3*2^z with
 *            x,y,z nonnegative integers. Computed by exhaustive search with
 *            64‑bit arithmetic; results fit into seq_t for the tested range
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers.
   Uses a simple bit‑wise method with 128‑bit multiplication to avoid overflow. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    for (int i = 31; i >= 0; --i) {
        uint64_t q = r + (1ULL << i);
        __uint128_t sq = (__uint128_t)q * q;
        if (sq <= n)
            r = q;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* for safety; offset is 1 */

    uint64_t nval = (uint64_t)n;
    uint64_t nSq  = nval * nval;

    uint64_t count = 0;

    for (uint64_t z = 0; ; ++z) {
        if (z >= 64)            /* avoid undefined shift */
            break;
        uint64_t powz = 1ULL << z;   /* 2^z */
        uint64_t sub  = 3ULL * powz; /* 3*2^z */
        if (sub > nSq)
            break;

        uint64_t r = nSq - sub;              /* remaining part: x^2 + 2*y^2 */
        uint64_t half = r >> 1;              /* floor(r/2), upper bound for y^2 */
        uint64_t yMax = isqrt_u64(half);      /* maximum y */

        for (uint64_t y = 0; y <= yMax; ++y) {
            uint64_t ySq   = y * y;
            uint64_t twoY2 = ySq << 1;
            uint64_t s     = r - twoY2;           /* candidate x^2 */
            uint64_t x     = isqrt_u64(s);
            if (x * x == s)
                ++count;                    /* valid representation found */
        }
    }

    return (seq_t)count;
}
