/*
 * A340459  a(n) is the sum of the numbers adjacent to n in a triangle in
 *          which the nonnegative integers are placed from top to bottom and
 *          from left to right.
 * https://oeis.org/A340459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the numbers adjacent to n in a triangle of
 *            nonnegative integers arranged row by row. The triangle is 0 1 2
 *            3 4 5 6 7 8 9 ... Adjacent nodes are the six immediate
 *            neighbours in the triangular lattice. The function works for any
 *            n ≥ 0 and is O(1)
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    /* floor(sqrt(x)) for 0 ≤ x < 2^68 (fits in 64 bits result).  */
    uint64_t lo = 0, hi = (1ULL << 34);           /* 2^34 > sqrt(max) */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 eight_n_plus_one = 8 * nn + 1;
    uint64_t sqrt_val = isqrt_uint128(eight_n_plus_one);
    uint64_t r = (sqrt_val - 1) / 2;                /* row */
    unsigned __int128 base_r = (unsigned __int128)r * (r + 1) / 2;
    uint64_t k = (uint64_t)(nn - base_r);           /* position in row */

    unsigned __int128 base_up   = (unsigned __int128)(r ? (r - 1) * r / 2 : 0);
    unsigned __int128 base_down = (unsigned __int128)(r + 1) * (r + 2) / 2;

    unsigned __int128 sum = 0;

    /* left neighbour */
    if (k > 0)
        sum += base_r + (k - 1);

    /* right neighbour */
    if (k < r)
        sum += base_r + (k + 1);

    /* upper-left and upper-right neighbours (if r > 0) */
    if (r > 0) {
        if (k > 0)
            sum += base_up + (k - 1);
        if (k < r)
            sum += base_up + k;
    }

    /* down-left and down-right neighbours (always exist) */
    sum += base_down + k;
    sum += base_down + (k + 1);

    return (seq_t)sum;
}
