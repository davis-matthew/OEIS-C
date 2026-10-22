/*
 * A398446  Number of permutations of [n] avoiding the pattern 1324 with the
 *          maximum element n in position n-2.
 * https://oeis.org/A398446
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n*binomial(2*n-6, n-3) + 4^(n-3) ) / 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n >= 3 (offset 3).  For n < 3 the value is undefined; return 0. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;  /* value below offset is not used */

    /* binomial(2*n-6, n-3) */
    seq_t bin = binomial(2 * n - 6, n - 3);

    /* term1 = n * binomial(...) */
    seq_t term1 = (seq_t)n * bin;

    /* term2 = 4^(n-3) = 2^(2*(n-3)) */
    seq_t term2 = pow2(2 * (n - 3));

    /* numerator = term1 + term2 */
    seq_t sum = term1 + term2;

    /* final division by 2 */
    return sum / 2;
}
