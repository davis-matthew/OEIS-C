/*
 * A363591  a(n) = 3*(3^(n-1) - 2^n + 1)/2 - binomial(n,2), n >= 3.
 * https://oeis.org/A363591
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3^(n-1) - 2^n + 1)/2 - binomial(n,2), n >= 3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"     /* pow2: 2^k */
#include "../utils/pow3.h"     /* pow3: 3^k   */
#include "../utils/binomial.h"/* binomial(n,k) */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* offset starts at n = 3 */

    /* compute the main term: 3*(3^(n-1) - 2^n + 1)/2 */
    seq_t p3  = pow3(n - 1);          /* 3^(n-1) */
    seq_t p2  = pow2(n);             /* 2^n   */

    /* (3^(n-1) - 2^n + 1)  is always even, so divide first to avoid overflow */
    seq_t diff = p3 - p2 + (seq_t)1;
    diff = diff / 2;                 /* (3^(n-1) - 2^n + 1)/2 */

    seq_t term = diff * (seq_t)3;     /* multiply by 3 */

    /* subtract binomial(n,2) */
    seq_t bin = binomial(n, 2);

    return term - bin;
}
