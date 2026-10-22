/*
 * A374383  Triangle T(n, k), n > 0, k = 0..n-1, read by rows; T(n, k) is the
 *          least base b >= 2 where n and k have different sums of digits.
 * https://oeis.org/A374383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A374383.
   a(n) is the flattening of the triangle T(n,k), n>0, 0<=k<n,
   where T(n,k) is the least base b>=2 in which n and k have
   different sums of digits.  The function finds the row and
   column corresponding to the flattened index n, then scans
   bases from 2 up to n+1 to locate the desired base. */
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for 64‑bit unsigned values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Returns a(n) for arbitrary n >= 1. */
seq_t a(idx_t n)
{
    /* 1. Find the triangle row r with r*(r+1)/2 >= n. */
    idx_t r = isqrt_u64(1 + 8*n);
    r = (r - 1) >> 1;              /* floor((sqrt(1+8n)-1)/2) */
    if (r < 1) r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* 2. Determine column k (0‑based) within row r. */
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev - 1;        /* 0 <= k < r */

    /* 3. Special case k == 0: the definition guarantees T(r,0)=2. */
    if (k == 0)
        return (seq_t)2;

    /* 4. Search for the smallest base b >= 2 where digit sums differ. */
    for (idx_t b = 2; b <= r + 1; ++b)
    {
        uint64_t bb = (uint64_t)b;
        uint64_t sum_r = 0, sum_k = 0;
        uint64_t x;

        x = (uint64_t)r;
        while (x)
        {
            sum_r += x % bb;
            x /= bb;
        }

        x = (uint64_t)k;
        while (x)
        {
            sum_k += x % bb;
            x /= bb;
        }

        if (sum_r != sum_k)
            return (seq_t)b;   /* base found */
    }

    /* 5. Fallback (should not occur): return the upper bound. */
    return (seq_t)(r + 1);
}
