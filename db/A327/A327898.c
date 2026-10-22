/*
 * A327898  a(n) is the nearest integer to the reciprocal of the difference
 *          between the square root of n and the nearest integer to this
 *          square root unless n is a perfect square, in which case a(n)
 *          equals 0.
 * https://oeis.org/A327898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round(1/(sqrt(n)-round(sqrt(n)))) for n not a perfect
 *            square, a(n)=0 otherwise
 * Range:     i8 n <= 4096, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit non‑negative x: returns floor(sqrt(x)). */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = (uint64_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Integer square root for 128‑bit value: returns floor(sqrt(x)). */
static uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = (uint64_t)1 << 39; /* sqrt(2^128-1) < 2^64, but 2^39 is safely above sqrt(2^128) */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) {            /* safety for indices below the offset */
        return (seq_t)0;
    }

    uint64_t r = isqrt_u64(n);                 /* floor(sqrt(n))          */
    unsigned __int128 rr = (unsigned __int128)r * r;
    if (rr == n) {                           /* perfect square -> 0       */
        return (seq_t)0;
    }

    /* Determine whether the nearest integer to sqrt(n) is r (positive delta)
       or r+1 (negative delta).  The boundary is r*r + r.              */
    bool negative = (n > (uint64_t)rr + r);
    uint64_t m = negative ? r + 1 : r;

    /* Compute s = floor(sqrt(n) * 2^32).                               */
    unsigned __int128 target = ((unsigned __int128)n) << 64;          /* n * 2^64 */
    uint64_t s = isqrt128(target);                                   /* floor(sqrt(target)) = floor(sqrt(n) * 2^32) */

    /* Diff = |s - m*2^32|;  this is |(sqrt(n)-m)*2^32|, always > 0.   */
    unsigned __int128 m_shift = ((unsigned __int128)m) << 32;
    __int128 diff_int = (__int128)s - (__int128)m_shift;
    if (diff_int < 0) diff_int = -diff_int;
    uint64_t diff = (uint64_t)diff_int;   /* 1 <= diff < 2^32 */

    /* Compute k ≈ round(2^32 / diff).  Use integer arithmetic with rounding. */
    unsigned __int128 num = (unsigned __int128)1 << 32;          /* 2^32   */
    unsigned __int128 k   = (num + (diff >> 1)) / diff;        /* nearest integer */

    /* Apply sign determined earlier.                                      */
    if (negative) {
        return (seq_t)(- (int64_t)k);
    } else {
        return (seq_t)(k);
    }
}
