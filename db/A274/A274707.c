/*
 * A274707  a(n) = A(n) if n is even else a(n) = A(n)*(n-1)/(n+1) with A(n) =
 *          ((n-1)!/ floor((n-1)/2)!^2).
 * https://oeis.org/A274707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274707
   Definition:
     a(1) = 0
     For n ≥ 2
        if n is even:  a(n) = (n/2) * binomial(n-1, n/2-1)
        if n is odd :  a(n) =           binomial(n-1, (n-1)/2-1)
   (binomial(n, k) = n!/(k!(n-k)!))
   The binomial() routine is supplied by the build utilities.
*/

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    if ((n & 1) == 0) {                    /* even n  */
        idx_t m = n >> 1;                  /* n/2  */
        seq_t val = binomial(n - 1, m - 1);
        val = val * (seq_t)m;              /* multiply by m = n/2 */
        return val;
    } else {                               /* odd n   */
        idx_t m = (n - 1) >> 1;            /* (n-1)/2 */
        return binomial(n - 1, m - 1);
    }
}
