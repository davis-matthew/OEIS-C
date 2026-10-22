/*
 * A375747  Table read by rows: T(n,k) for n >= 3 and k=1,2,...,2*n-1 are the
 *          distinct eigenvalues of the twisted odd graph O^(sigma)_n.
 * https://oeis.org/A375747
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   a(n) is read row‑by‑row from the array T where T(r,1)=r,
 *            T(r,k)=r+1-k for 2 ≤ k ≤ r, T(r,k)=r-k for r+1 ≤ k ≤ 2r-1. The
 *            row index r is the smallest integer with r²-4 > n-3, and the
 *            column k = n - ((r-1)²-1)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for unsigned 64‑bit values.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 4294967295ULL;      /* floor(sqrt(2^64-1)) = 2^32-1 */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* for indices below the offset, the value is undefined; return 0 */
    if (n < 3)
        return (seq_t)0;

    /* Convert to a 0‑based position inside the "table". */
    idx_t pos = n - 3;               /* 0 for a(3), 1 for a(4), … */

    /* Find the row r: the smallest integer with r²-4 > pos. */
    idx_t target = pos + 4;          /* r² > target-1  =>  r > sqrt(target-1) */
    idx_t r = floor_isqrt_u64(target);   /* floor(sqrt(target)) */
    /* If r²-4 <= pos, r must be increased by one. */
    __uint128_t sq = (__uint128_t)r * r;
    if (sq - 4 <= pos)
        ++r;                         /* r is now ≥ 3 */

    /* Column index k (1‑based) within the row. */
    idx_t k = n - ((r - 1)*(r - 1)) + 2;   /* formula from the description */

    /* Compute the table value T(r,k). */
    seq_t sr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t val;

    if (k == 1)
        val = sr;                     /* T(r,1) = r */
    else if (k <= sr)
        val = sr + 1 - kk;             /* T(r,k)=r+1-k for 2≤k≤r */
    else
        val = sr - kk;                 /* T(r,k)=r-k   for r+1≤k≤2r-1 */

    return val;
}
