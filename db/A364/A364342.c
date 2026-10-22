/*
 * A364342  a(n) is the number of base-10 nonbouncy numbers below 10^n.
 * https://oeis.org/A364342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binomial(n+9,n) + (n+1)*binomial(n+10,n+1)/10 - 10*n - 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside the sequence offset */

    seq_t comb1 = binomial(n + 9, n);    /* binomial(n+9, n)   */
    seq_t comb2 = binomial(n + 10, n + 1);  /* binomial(n+10, n+1) */

    seq_t term2 = ((seq_t)(n + 1) * comb2) / (seq_t)10;
    seq_t result = comb1 + term2;
    result -= (seq_t)(10 * n + 1);        /* subtract 10*n + 1   */

    return result;
}
