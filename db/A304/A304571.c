/*
 * A304571  Triangle read by rows: T(n,k) = 1 if gcd(k,n) > 1 and n mod k !=
 *          0.
 * https://oeis.org/A304571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9782, u16 n <= 9782, u32 n <= 9782, u64 n <= 9782,
 *            u128 n <= 9782, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304571: read by rows of the triangle
   T(r,k)=1 iff gcd(k,r)>1 AND r mod k ≠ 0.
   a(n) is the n‑th element of the flattened triangle.
   The function finds the row r and column k for index n
   and computes the condition. */

#include <stdint.h>
#include "../utils/gcd.h"

/* integer square root of a 64‑bit unsigned integer,
   using binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute a(n) for the OEIS sequence A304571 */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but guard anyway */
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    if (r * (r + 1) / 2 < n)
        ++r;

    /* Previous cumulative count */
    idx_t prev_sum = (r - 1) * r / 2;
    /* Column index (1‑based) */
    idx_t k = n - prev_sum;

    /* Evaluate the triangle condition */
    seq_t rn = (seq_t)r;
    seq_t ks = (seq_t)k;
    seq_t g = gcd(rn, ks);
    if (g > (seq_t)1 && (rn % ks) != (seq_t)0)
        return (seq_t)1;
    return (seq_t)0;
}
