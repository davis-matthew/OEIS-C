/*
 * A343698  a(n) is the number of preference profiles in the stable marriage
 *          problem with n men and n women such that there are n pairs of
 *          soulmates (people who rank each other first).
 * https://oeis.org/A343698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)!^(2n) * n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   factorial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* only n >= 1 is defined */
    seq_t fact_n   = factorial(n);    /* n!          */
    seq_t fact_nm1 = factorial(n-1);  /* (n-1)!     */
    seq_t exp      = (seq_t)(2*n);    /* 2n          */
    seq_t pow_term = pow_seq_large(fact_nm1, exp); /* (n-1)!^(2n) */
    return fact_n * pow_term;         /* final product */
}
