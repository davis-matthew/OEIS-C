/*
 * A244465  Expansion of f(-x^3, -x^5) in powers of x where f() is Ramanujan's
 *          two-variable theta function.
 * https://oeis.org/A244465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * f(-x^3, -x^5) = Sum_{k∈ℤ} (-1)^k
 *            * x^(4k^2 - k). * This coefficient is 1 if 4k^2 - k = n with
 *            even k, * -1 if 4k^2 - k = n with odd k, otherwise 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for uint64_t (binary search) */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t lo = 0, hi = x >> 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Compute d = 1 + 16 * n */
    uint64_t nn = (uint64_t)n;
    uint64_t d  = 1ULL + 16ULL * nn;

    /* sqrt(d) */
    uint64_t s = isqrt_uint64(d);
    if ((unsigned __int128)s * s != d)
        return 0;  /* no integer solution for k */

    seq_t coeff = 0;

    /* first branch: k = (1 + s) / 8 */
    if (((1ULL + s) % 8ULL) == 0) {
        long long k = (long long)(1ULL + s) / 8LL;
        coeff = ( (k & 1LL) == 0 ) ? 1 : -1;
    }
    /* second branch: k = (1 - s) / 8 (negative k) */
    else if (((1ULL - s) % 8ULL) == 0) {
        long long k = (long long)(1ULL - s) / 8LL;
        coeff = ( (k & 1LL) == 0 ) ? 1 : -1;
    }

    return coeff;
}
