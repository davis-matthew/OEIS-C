/*
 * A297151  a(n) = Sum_{i=0..n} Sum_{j=0..n} Sum_{k=0..n}
 *          binomial(n,i)*binomial(n,j)*binomial(n,k)*max(i,j,k).
 * https://oeis.org/A297151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^(3*n-1) + 3*n*2^(n-2)*binomial(2*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    n*2^(3*n-1)  = n << (3*n-1)      */
/*    3*n*2^(n-2)*binomial(2*n, n)   = 3*n*(binomial(2*n, n)/2) << (n-1) */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 0 */

    /* first term: n * 2^(3*n-1) */
    idx_t exp1 = 3 * n - 1;                 /* exp1 >= 0 for n >= 1 */
    seq_t term1 = (seq_t)n * pow2(exp1);

    /* second term: 3*n*2^(n-2)*binomial(2*n,n) */
    seq_t bin = binomial(2 * n, n);        /* binomial(2*n, n) is even for n >= 1 */
    seq_t half = bin >> 1;                 /* divide by 2 (exact) */

    seq_t term2 = (seq_t)3 * (seq_t)n * half;  /* 3*n* (bin/2)   */
    term2 = term2 << (n - 1);               /* multiply by 2^(n-1) */

    return term1 + term2;
}
