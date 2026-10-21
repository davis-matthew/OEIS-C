/*
 * A286754  Positions of 1 in A286752; complement of A286753.
 * https://oeis.org/A286754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (floor(n * φ) mod 2) * where φ = (1 + √5)/2. The
 *            parity of floor(n*φ) is found by * using the integer relation
 *            floor(n*φ) = (n + floor(n*√5)) / 2. * floor(n*√5) is obtained as
 *            the integer square‑root of 5*n*n
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Integer square root of a 128‑bit integer:
 * returns floor(sqrt(x)).  Works for x up to (2^64‑1)^2. */
static uint64_t isqrt_uint128(__int128 x)
{
    uint64_t low  = 0;
    uint64_t high = ULLONG_MAX;   /* 2^64‑1, safe upper bound for our use */
    uint64_t ans  = 0;

    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq  = (__int128)mid * mid;

        if (sq <= x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Calculate floor(n * √5) by integer sqrt of 5*n*n */
    __int128 prod  = (__int128)n * n * 5;
    uint64_t sq    = isqrt_uint128(prod);

    /* floor(n*φ) = (n + floor(n*√5)) / 2  */
    uint64_t nphi  = ((uint64_t)n + sq) >> 1;

    /* Parity of floor(n*φ) */
    uint64_t eps   = nphi & 1ULL;

    /* a(n) = 2*n - eps  */
    seq_t result   = (seq_t)n * 2 - eps;
    return result;
}
