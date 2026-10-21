/*
 * A285192  Array read by antidiagonals: T(n,k) = n*k*(3+n*k)/2 (n >= 0, k >=
 *          0).
 * https://oeis.org/A285192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,col) where T(n,k)=n*k*(3+n*k)/2, read by
 *            antidiagonals of the infinite array
 * Range:     u8 n <= 58, u16 n <= 759, u32 n <= 9314, u64 n <= 9314,
 *            u128 n <= 9314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 64‑bit unsigned integers.  */
static inline uint64_t floor_sqrt_uint64(uint64_t n)
{
    uint64_t lo = 0;
    uint64_t hi = ((uint64_t)1 << 32);          /* > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)           /* mid*mid > n, without overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Return the nth term of A285192. */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that the cumulative number of elements
       up to diagonal d-1 is ≤ n < cumulative up to diagonal d.  For
       non‑negative indices, cumulative count up to diagonal d is
       C(d) = (d+1)(d+2)/2.  The smallest d satisfying C(d) > n is
       d = floor((sqrt(8*n+1)-1)/2). */
    uint64_t d = (floor_sqrt_uint64(8 * (uint64_t)n + 1) - 1) / 2;

    /* Number of elements up to the previous diagonal (d-1). */
    idx_t prev_cum = (idx_t)(d * (d + 1) / 2);

    /* Offset within diagonal d.  row = offset, col = d - offset. */
    idx_t offset = n - prev_cum;
    idx_t row = offset;
    idx_t col = d - offset;

    /* Compute T(row,col) = row*col*(3 + row*col)/2.
       Use a wider unsigned integer for the intermediate products to
       avoid overflow when seq_t is small. */
    uint64_t nk   = (uint64_t)row * (uint64_t)col;
    uint64_t val  = nk * (nk + 3ULL) / 2ULL;
    return (seq_t)val;
}
