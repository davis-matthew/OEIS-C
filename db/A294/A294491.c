/*
 * A294491  Number of length 2n n-ary words that can be built by repeatedly
 *          inserting doublets into the initially empty word.
 * https://oeis.org/A294491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-1} binomial(2*n, j)*(n-j)*(n-1)^j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    idx_t two_n = n << 1;          /* 2*n  */
    seq_t base = (seq_t)(n - 1);   /* n-1  */
    seq_t pow = 1;                 /* base^0 initially */

    for (idx_t j = 0; j < n; ++j) {
        seq_t bin = binomial(two_n, j);        /* C(2n, j) */
        seq_t term = bin * (seq_t)(n - j);     /* * (n-j)   */
        term *= pow;                          /* * base^j  */
        sum += term;

        pow *= base;   /* prepare for next exponent j+1 */
    }

    return sum;
}
