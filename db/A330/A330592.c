/*
 * A330592  a(n) is the number of subsets of {1,2,...,n} that contain exactly
 *          two odd numbers.
 * https://oeis.org/A330592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * binomial((n+1)/2, 2) * 2^((n-1)/2) if n is odd, *
 *            binomial(n/2, 2) * 2^(n/2) if n is even. * This implements the
 *            OEIS sequence A330592
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 47, u64 n <= 107, u128 n <= 231,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases: a(1)=a(2)=0 are covered by the formula, but
     * returning early avoids a tiny overhead for very small n.      */
    if (n < 3)
        return 0;

    idx_t m;        /* number of odds (or half of n)          */
    idx_t exp;      /* exponent for the power of two          */

    if (n & 1ULL) {             /* n is odd */
        m   = (n + 1) >> 1;     /* (n+1)/2 */
        exp = (n - 1) >> 1;     /* (n-1)/2 */
    } else {                    /* n is even */
        m   = n >> 1;           /* n/2 */
        exp = n >> 1;           /* n/2 */
    }

    seq_t bin = binomial(m, 2);     /* C(m,2)                            */
    if (!bin)                       /* fast exit for small m < 2         */
        return 0;

    seq_t pw   = pow2(exp);         /* 2^exp                            */
    return bin * pw;                /* final result                      */
}
