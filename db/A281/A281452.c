/*
 * A281452  Expansion of f(x, x) * f(x^5, x^13) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A281452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(k,l) ∈ ℤ² : n = k² + 9·l² + 4·l}| implemented as a
 *            convolution of the power series Σ_{k∈ℤ} x^{k²} · Σ_{l∈ℤ}
 *            x^{9·l²+4·l}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Common integer types (supplied by the build system) */
#include <stdint.h>
#include <limits.h>

/* integer square root of a 64‑bit unsigned integer using binary search */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 32;           /* √(2⁶⁴‑1) < 2³²   */
    uint64_t ans  = 0;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __int128 sq  = (__int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* test whether m can be written as 9t²+4t for some integer t */
static int is_b_rep(uint64_t m)
{
    if (m == 0) return 1;          /* t = 0 */

    __int128 D128 = (__int128)36 * m + 16;   /* discriminant 16+36m */

    /* if D exceeds 64‑bit range, the value cannot be a square */
    if (D128 > (__int128)UINT64_MAX) return 0;

    uint64_t D  = (uint64_t)D128;
    uint64_t s  = isqrt_u64(D);

    /* check that D is a perfect square */
    if ((__int128)s * s != D128) return 0;

    /* check the two possible roots of 9t²+4t = m */
    if (s >= 4) {
        uint64_t r = s - 4;
        if (r % 18 == 0) {
            uint64_t t = r / 18;
            if (t >= 1) return 1;
        }
    }
    uint64_t r2 = s + 4;
    if (r2 % 18 == 0) {
        uint64_t t = r2 / 18;
        if (t >= 1) return 1;
    }
    return 0;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    seq_t result = 0;
    uint64_t N = (uint64_t)n;                /* flatten idx_t to 64‑bit */

    uint64_t maxk = isqrt_u64(N);
    for (uint64_t k = 0; k <= maxk; ++k) {
        uint64_t k2 = k * k;
        uint64_t m  = N - k2;
        if (is_b_rep(m)) {
            /* k = 0 gives one pair (k,l); k > 0 gives two pairs (±k,l) */
            result += (k == 0) ? 1 : 2;
        }
    }
    return result;
}
