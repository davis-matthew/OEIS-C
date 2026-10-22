/*
 * A359842  a(n) = Sum_{k=0..n} binomial(n*k,n+k).
 * https://oeis.org/A359842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n*k, n+k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial utility provides an efficient implementation of binomial
   coefficients for idx_t arguments.  The function binomial(idx_t a, idx_t b)
   returns the binomial coefficient C(a,b) as a seq_t value. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1; the sum also evaluates to 1 for n = 0. */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t ak = n * k;          /* a = n*k   */
        idx_t bk = n + k;          /* b = n+k   */
        sum += binomial(ak, bk);
    }
    return sum;
}
