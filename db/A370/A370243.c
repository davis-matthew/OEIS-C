/*
 * A370243  Coefficient of x^n in the expansion of ( 1/(1-x) * (1+x^2)^2 )^n.
 * https://oeis.org/A370243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n,k) * binomial(2*n-2*k-1,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial utility is provided by the build environment. */
#include "../utils/binomial.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition; the general formula would access
       binomial(-1,0) which is undefined. */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t max_k = n >> 1;           /* floor(n/2) */
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t t1 = binomial(2 * n, k);
        seq_t t2 = binomial(2 * n - 2 * k - 1, n - 2 * k);
        sum += t1 * t2;
    }
    return sum;
}
