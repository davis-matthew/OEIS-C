/*
 * A361474  a(n) = 1*binomial(n,2) + 3*binomial(n,3) + 6*binomial(n,4) +
 *          10*binomial(n,5).
 * https://oeis.org/A361474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*binomial(n,2) + 3*binomial(n,3) + 6*binomial(n,4) +
 *            10*binomial(n,5)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   _binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_binom.h"

seq_t a(idx_t n)
{
    /* Ensure values are computed in seq_t; casting n to seq_t where needed */
    seq_t result = 0;

    /* term for binomial(n,2) with coefficient 1 */
    result += _binom((uint64_t)n, 2);

    /* term for binomial(n,3) with coefficient 3 */
    result += 3 * _binom((uint64_t)n, 3);

    /* term for binomial(n,4) with coefficient 6 */
    result += 6 * _binom((uint64_t)n, 4);

    /* term for binomial(n,5) with coefficient 10 */
    result += 10 * _binom((uint64_t)n, 5);

    return result;
}
