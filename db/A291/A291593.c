/*
 * A291593  Number of (non-null) connected induced subgraphs in the n X n rook
 *          complement graph.
 * https://oeis.org/A291593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2) - 2*n*(2^n-n-1) - n^2*(2^(n-1)-1)^2 -
 *            binomial(n,2)^2 - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* n as seq_t          */
    seq_t n2  = nn * nn;           /* n^2                 */
    seq_t t1  = big_pow((seq_t)2, n2);                     /* 2^(n^2)             */

    seq_t pow2n = pow2(n);                                   /* 2^n                 */
    seq_t sub1  = pow2n - nn - (seq_t)1;                     /* 2^n - n - 1         */
    seq_t t2    = (seq_t)2 * nn * sub1;                       /* 2*n*(2^n-n-1)       */

    seq_t pow2n_1 = pow2(n - 1);                             /* 2^(n-1)             */
    seq_t sub2    = pow2n_1 - (seq_t)1;                       /* 2^(n-1)-1           */
    seq_t sub2_sq = sub2 * sub2;                              /* (2^(n-1)-1)^2       */
    seq_t t3      = n2 * sub2_sq;                            /* n^2*(2^(n-1)-1)^2   */

    seq_t binom_n2  = binomial(n, 2);                         /* binomial(n,2)       */
    seq_t binom_sq  = binom_n2 * binom_n2;                    /* binomial(n,2)^2     */

    seq_t result = t1 - t2 - t3 - binom_sq - (seq_t)1;       /* combine all terms   */
    return result;
}
