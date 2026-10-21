/*
 * A287770  Positions of 1 in A287769; complement of A276855.
 * https://oeis.org/A287770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(15 - sqrt(5))/10)
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = floor((15*n - floor(n*sqrt(5)))/10)  */
/*           = (15*n - floor(n*sqrt(5)))/10  with a correction
 *             if the remainder of the division by 10 is zero.   */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = 1;
    /* find high such that high*high > x */
    while (high <= x / high)
        high <<= 1;
    /* binary search on [low,high) */
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* OEIS A287770: positions of 1 in A287769. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, below offset is undefined */
        return 0;

    /* promote to 128‑bit to avoid overflow for n up to ~8e18 */
    unsigned __int128 nn  = n;
    /* floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
    unsigned __int128 t   = 5 * nn * nn;
    unsigned __int128 fns = isqrt128(t);

    /* tmp = 15*n - floor(n*sqrt(5)) */
    unsigned __int128 tmp = 15 * nn - fns;

    /* base = floor(tmp/10) */
    seq_t base = (seq_t)(tmp / 10);

    /* subtract 1 if the remainder of tmp / 10 is zero */
    if ((tmp % 10) == 0)
        --base;

    return base;
}
