/*
 * A382087  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * B(x)^2)
 *          ), where B(x) = 1 + x*B(x)^3 is the g.f. of A001764.
 * https://oeis.org/A382087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382087: a(0)=1.
   Formula (from SPDX):
   a(n) = Σ_{j=0..n-1} (n-1)!/j! * binomial(3n-j-2, n-1-j) * (n+1)^j   for n>0
*/
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/factorial.h"  /* seq_t factorial(idx_t n); */

seq_t a(idx_t n)
{
    if (n == 0ull)
        return 1ull;          /* a(0) = 1 */

    /* (n-1)!  (used as the initial value of C = (n-1)!/0! ) */
    seq_t C = factorial(n - 1ull);   /* C = (n-1)!   */

    seq_t pow = 1ull;   /* (n+1)^j  */
    seq_t sum = 0ull;   /* accumulating the sum */

    for (idx_t j = 0ull; j < n; ++j)
    {
        /* binomial(3n - j - 2, n - 1 - j) */
        seq_t bin = binomial(3ull * n - j - 2ull, n - 1ull - j);

        /* term = C * pow * bin  (C = (n-1)!/j!) */
        seq_t term = C;
        term *= pow;
        term *= bin;

        sum += term;

        /* update C for next j: C_{j+1} = C_j / (j+1) */
        C /= (j + 1ull);

        /* update pow for next j: pow = pow * (n+1) */
        pow *= (n + 1ull);
    }

    return sum;          /* this is a(n) */
}
