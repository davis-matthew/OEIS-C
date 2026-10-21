/*
 * A249133  Triangle read by rows: interleaving successive pairs of rows of
 *          Sierpiński's triangle.
 * https://oeis.org/A249133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9960, u16 n <= 9960, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   binom_mod2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   Let r = floor(sqrt(n)) and pos = n - r*r   (0 <= pos <= 2*r).
 *   If r == 0 or pos == 2*r: a(n) = 1.
 *   If pos is even:          a(n) = C(r, pos/2)   (mod 2).
 *   If pos is odd :          a(n) = C(r-1, pos/2) (mod 2).
 *
 *   Here C(n,k) is the binomial coefficient.  Modulo 2 a binomial
 *   coefficient is 1 iff (k & (n-k)) == 0.
 */

#include <stdint.h>
#include "../utils/binom_mod2.h"

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    if (x < 2) return x;          /* 0 -> 0, 1 -> 1 */
    idx_t low  = 1;
    idx_t high = (idx_t)1ULL << 32;   /* high > sqrt(2^64-1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* high is the floor of sqrt(x) */
}

seq_t a(idx_t n)
{
    /* Determine the row index */
    idx_t r   = floor_sqrt_u64(n);          /* r = floor(sqrt(n))   */
    idx_t pos = n - r * r;                  /* position in the row   */

    /* Special case: the first element of each interleaved row */
    if (r == 0 || pos == 2 * r) {
        return (seq_t)1;
    }

    /* Even or odd position */
    if ((pos & 1) == 0) {                   /* even position  */
        idx_t i = pos >> 1;                 /* i = pos/2        */
        return binom_mod2(r, i);
    } else {                                /* odd position   */
        idx_t i = pos >> 1;                 /* i = pos/2        */
        return binom_mod2(r - 1, i);
    }
}
