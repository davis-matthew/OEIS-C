/*
 * A244148  The number of ways one can assign values to n arrays
 *          a_{1},...,a_{n} of increasing size (size of a_{1} is 1, size of
 *          a_{2} is 2, ..., size of a_{n} is n) using the numbers 1, ...,
 *          n*(n+1)/2, distinctly, such that the positions of array a_{i} can
 *          only be assigned values in the interval ((n+1)-i),....
 * https://oeis.org/A244148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Prod_{k=1..n} (k! * binomial((n^2 - 3*n + 5*k - k^2)/2 ,
 *            k))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n = 0 we can return 0 (behaviour below offset does not matter) */
    if (n == 0)
        return 0;

    seq_t result = 1;   /* product of all terms */
    seq_t fact_k = 1;   /* current k! */

    for (idx_t k = 1; k <= n; ++k)
    {
        fact_k = fact_k * (seq_t)k;                 /* update k! */

        idx_t num = (n * n - 3 * n + 5 * k - k * k) / 2;
        seq_t bin = binom(num, k);                 /* binomial coefficient */

        seq_t term = fact_k * bin;                  /* k! * C(num,k) */

        result = result * term;                     /* accumulate product */
    }

    return result;
}
