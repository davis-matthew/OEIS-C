/*
 * A286655  Characteristic sequence of the Beatty sequence, A022842, of
 *          sqrt(8).
 * https://oeis.org/A286655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - floor((n+1)*(1-1/r)) + floor(n*(1-1/r)), r = sqrt(8)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root (floor) for unsigned 64‑bit values */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 m2 = (__int128)mid * mid;
        if (m2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* values below the offset are irrelevant */
        return (seq_t)0;

    /* Compute bounds for k such that n^2 <= 8*k^2 < (n+1)^2 */
    __int128 nn  = (__int128)n;
    idx_t n2     = (idx_t)(nn * nn);          /* n^2  */
    idx_t n1     = n + 1;
    __int128 n1n = (__int128)n1 * n1;
    idx_t n1sq   = (idx_t)n1n;               /* (n+1)^2  */

    /* ceil(n^2 / 8)  =  (n^2 + 7) / 8  */
    idx_t low = (n2 + 7) / 8;
    /* floor( ((n+1)^2 - 1) / 8 )  */
    idx_t high = (n1sq - 1) / 8;

    /* ceil_sqrt(low) */
    idx_t k = isqrt_u64(low);
    if ((__int128)k * k < low)
        k++;

    if ((__int128)k * k <= high)
        return (seq_t)1;
    return (seq_t)0;
}
