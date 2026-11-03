/*
 * A329854  Triangle read by rows: T(n,k) = ((n - k)*(n + k - 1) + 2)/2, 0 <=
 *          k <= n.
 * https://oeis.org/A329854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(r,c) = ((r-c)*(r+c-1)+2)/2
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2 */
    seq_t val = (seq_t)8 * (seq_t)n + 1;      /* 8*n + 1 */
    seq_t s   = floor_sqrt(val);             /* floor sqrt(8*n+1) */
    seq_t r   = (s - 1) / 2;                  /* row index */
    seq_t tri = r * (r + 1) / 2;              /* triangular number T(r) */
    seq_t c   = (seq_t)n - tri;              /* column index */

    /* Handle the only place where (r+c-1) would be negative: n = k = 0 */
    if (r == 0 && c == 0)
        return (seq_t)1;

    seq_t term1 = r - c;                      /* r - c (non‑negative) */
    seq_t term2 = r + c - 1;                  /* r + c - 1 (now non‑negative) */
    seq_t numer = term1 * term2 + 2;          /* numerator of the formula */
    return numer / 2;                         /* final value T(r,c) */
}
