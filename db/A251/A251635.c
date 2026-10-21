/*
 * A251635  Riordan array (1-2*x,x), inverse of Riordan array (1/(1-2*x), x) =
 *          A130321.
 * https://oeis.org/A251635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = 1 if 1+8*(n+1) is a perfect square and (sqrt-1) is even;
 *            * a(n) = -2 if 9+8*(n+1) is a perfect square and (sqrt-3) is
 *            even; * otherwise a(n) = 0. * This follows from the explicit
 *            positions of 1's and –2's in the sequence
 * Range:     i8 n <= 18, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root for 64‑bit values (floor of sqrt), using binary search */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);          /* sqrt(2^64-1) < 2^32          */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

static inline int is_square_u64(idx_t x)
{
    idx_t r = floor_sqrt_u64(x);
    return (unsigned __int128)r * r == x;
}

seq_t a(idx_t n)
{
    /* use seq_t for intermediate arithmetic to avoid overflow for large n */
    seq_t t = (seq_t)n + 1;            /* t = n + 1                                 */
    seq_t s1_seq = t * 8 + 1;          /* s1 = 1 + 8*(n+1)                         */
    seq_t s2_seq = t * 8 + 9;          /* s2 = 9 + 8*(n+1)                         */

    /* first check the positions of the 1's */
    if (is_square_u64((idx_t)s1_seq)) {
        idx_t sqrt1 = floor_sqrt_u64((idx_t)s1_seq);
        if ((sqrt1 - 1) % 2 == 0) {    /* (sqrt1-1) divisible by 2               */
            return (seq_t)1;
        }
    }

    /* now check the positions of the -2's */
    if (is_square_u64((idx_t)s2_seq)) {
        idx_t sqrt2 = floor_sqrt_u64((idx_t)s2_seq);
        if ((sqrt2 - 3) % 2 == 0) {    /* (sqrt2-3) divisible by 2               */
            return (seq_t)-2;
        }
    }

    /* all other positions are 0 */
    return (seq_t)0;
}
