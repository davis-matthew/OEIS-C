/*
 * A340945  List of Y-coordinates of point moving along one of the arms of a
 *          counterclockwise square spiral with four arms; A340944 gives
 *          X-coordinates.
 * https://oeis.org/A340945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = a single Y‑coordinate of the counterclockwise square
 *            spiral, as defined by OEIS A340945
 * Range:     i8 n <= 9378, i16 n <= 9378, i32 n <= 9378, i64 n <= 9378,
 *            i128 n <= 9378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by: let s = round(sqrt(n)), r = n - s^2; then
   a(n) = { -s        if s%4==0,
            r         if s%4==1,
            s         if s%4==2,
           -r         if s%4==3 } */

/* Integer square‑root floor for 64‑bit unsigned integers. */
#include <stdint.h>
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;          /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod == (unsigned __int128)n)
            return mid;
        if (prod < (unsigned __int128)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;          /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* s = round(sqrt(n))  (nearest integer to sqrt) */
    idx_t s = isqrt_u64(n);
    if ((__int128)n >= (__int128)s * s + s + 1)
        ++s;

    /* r = n - s^2 (can be negative) */
    __int128 diff = (__int128)n - (__int128)s * s;
    seq_t r = (seq_t)diff;

    seq_t ans;
    switch (s & 3)     /* s % 4 */
    {
        case 0: ans = -(seq_t)s;   break;   /* * 1 */
        case 1: ans = r;           break;   /* * i */
        case 2: ans = (seq_t)s;    break;   /* * -1 */
        case 3: ans = -r;          break;   /* * -i */
    }
    return ans;
}
