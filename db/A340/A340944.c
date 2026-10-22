/*
 * A340944  List of X-coordinates of point moving along one of the arms of a
 *          counterclockwise square spiral with four arms; A340945 gives
 *          Y-coordinates.
 * https://oeis.org/A340944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = Re( (r - s*i) * i^s ), s = round(sqrt(n)), r = n - s^2 *
 *            * This implements the equivalent rules from the OEIS entry: *
 *            a(n) = A340945(n) iff n∈A002939, * a(n) = -A340945(n) iff n =
 *            4*x^2 - 6*x + 2, * a(n) = 0 iff n is an even square, * a(n) = Re
 *            z where z = (r - s*i)*i^s with s = round(sqr...
 * Range:     i8 n <= 9494, i16 n <= 9494, i32 n <= 9494, i64 n <= 9494,
 *            i128 n <= 9494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of sqrt for uint64_t, using binary search and 128‑bit
 * intermediate to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0;
    idx_t hi = ((idx_t)1) << 32; /* ceiling of sqrt(UINT64_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Step 1: floor sqrt */
    idx_t f = floor_sqrt_u64(n);
    /* Step 2: remainder to current square */
    idx_t d = n - f * f;          /* 0 ≤ d < 2f+1 */
    /* Step 3: round to nearest integer sqrt */
    idx_t s = f;
    if (((d << 1) > (2 * f + 1)))   /* d > (2f+1)/2  => round up */
        s = f + 1;

    /* Step 4: compute r = n - s^2 using only the known difference */
    seq_t r = (seq_t)d;
    if (s != f)
        r = r - (seq_t)(2 * f + 1);

    /* Step 5: rotate by i^s and take real part */
    seq_t result;
    switch ((int)(s & 3)) {
        case 0:  result = r;          break;   /* i^0 = 1        */
        case 1:  result = (seq_t)s;    break;   /* i^1 = i        */
        case 2:  result = -r;          break;   /* i^2 = -1       */
        case 3:  result = -(seq_t)s;   break;   /* i^3 = -i       */
    }
    return result;
}
