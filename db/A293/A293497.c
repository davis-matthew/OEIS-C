/*
 * A293497  Triangular array read by rows: row n >= 1 is the list of integers
 *          from 0 to 2n-1.
 * https://oeis.org/A293497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n - t*(t-1), where t = floor(sqrt(n+1)+1/2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit value: floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32;          /* 2^32  >= sqrt(UINT64_MAX) */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1; /* upper mid to avoid infinite loop */
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute t = floor(sqrt(n+1)+1/2) without floating point. */
    idx_t n1 = n + 1;                     /* n+1  (no overflow for typical n) */
    idx_t s  = isqrt_u64(n1);             /* floor(sqrt(n+1))  */

    /* Determine whether to round up: compare 4*(n+1) with (2*s+1)^2. */
    unsigned __int128 lhs = (unsigned __int128)4 * (unsigned __int128)n1;
    unsigned __int128 rhs = (unsigned __int128)((unsigned __int128)(s << 1) + 1) *
                            (unsigned __int128)((unsigned __int128)(s << 1) + 1);
    idx_t t = s + ((lhs >= rhs) ? 1 : 0);

    /* Compute a(n) = n - t*(t-1).  The product fits in 64 bits. */
    seq_t tt   = (seq_t)t;
    seq_t prod = tt * (tt - (seq_t)1);
    seq_t res  = (seq_t)n - prod;
    return res;
}
