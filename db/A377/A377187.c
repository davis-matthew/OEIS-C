/*
 * A377187  Triangle read by rows: T(n,k) = numerator((n^2 + k)/(n^2 - k)).
 * https://oeis.org/A377187
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = numerator( (r^2 + k)/(r^2 - k) )
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              where r is the row number (starting at 2),   */
/*              k is the column number (starting at 2),      */
/*              and the triangle is read by rows              */

#include <stdint.h>
#include "../utils/gcd.h"

/* integer square root: floor(sqrt(x)) for 0 <= x < 2^64 */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    if (high > 0xFFFFFFFFULL) high = 0xFFFFFFFFULL;   /* high^2 still fits in 64 bits */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* offset is 2; behavior for n<2 is undefined */

    idx_t p = n - 2; /* zero‑based position in the triangle stream */

    /* find the smallest r >= 2 such that r*(r-1)/2 > p  */
    idx_t tmp = (idx_t)((__uint128_t)1 + (__uint128_t)8 * p);
    idx_t d   = floor_sqrt_u64(tmp);
    idx_t r   = (d + 1) >> 1;              /* initial estimate */
    if ( (r * (r - 1)) / 2 <= p ) r++;     /* adjust if necessary */

    idx_t prev = ((r - 2) * (r - 1)) / 2;  /* cumulative count up to row r-1 */
    idx_t k    = p - prev + 2;             /* column number (>=2) */

    seq_t row    = (seq_t)r;
    seq_t row_sq = row * row;
    seq_t num    = row_sq + (seq_t)k;
    seq_t den    = row_sq - (seq_t)k;

    seq_t g = gcd(num, den);
    return num / g;
}
