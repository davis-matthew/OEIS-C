/*
 * A291699  a(n) = n^n*(2*n)!/(n!*(n + 1)!).
 * https://oeis.org/A291699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n * Catalan(n) = n^n * binomial(2*n,n)/(n+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* 0^0 is defined as 1 in this sequence. */

    /* Compute n^n.  Use big_pow for efficient exponentiation. */
    seq_t pow_val = big_pow((seq_t)n, (seq_t)n);

    /* Catalan number C_n = binomial(2*n,n)/(n+1).  The utility
       function catalan(n) returns this value as seq_t. */
    seq_t cat_val = catalan(n);

    /* Final result: a(n) = n^n * Catalan(n).  The multiplication
       is performed in seq_t arithmetic; intermediate overflow is
       acceptable only up to the maximum value representable by
       seq_t, as required by the test harness. */
    return pow_val * cat_val;
}
