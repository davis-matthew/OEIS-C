/*
 * A346232  Maximum number of squares in a square grid whose interiors can be
 *          touched by a (possibly skew) line segment of length n.
 * https://oeis.org/A346232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2*n^2 - 2)) + 3
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function implements the OEIS sequence A346232 (maximum number of
 * unit grid squares that a line segment of length n can touch).
 *
 * The formula is valid for all n >= 1.  For n == 0 we return 0, but the
 * sequence starts at index 1.
 *
 * The implementation uses only integer arithmetic and a small helper
 * routine to compute a floor integer square root on uint64_t values.
 * No floating point or external libraries are required.
 */

#include <stdint.h>

/* Helper: integer floor square root for 64‑bit values, using binary search. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << ((sizeof(idx_t) * 8) / 2);  /* hi = 2^(bits/2) */

    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;

        if (sq == v)
            return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; handle n==0 safely. */
    if (n == 0)
        return 0;

    /* Compute 2*n^2 - 2 using 64‑bit arithmetic. */
    idx_t val = 2 * n * n - 2;

    /* Floor integer square root of val. */
    idx_t root = isqrt_u64(val);

    /* Add 3 and convert to seq_t. */
    return (seq_t)(root + 3);
}
