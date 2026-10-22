/*
 * A321269  Number of permutations on [n] whose up-down signature has
 *          nonnegative partial sums and which have exactly three descents.
 * https://oeis.org/A321269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A321269
   Formula (for n > 3):
     a(n) = 4^n
            - 4*n*3^(n-1)
            + 9*binom(n,2)*2^(n-2)
            - binom(n,3)*2^(n-2)
            - 2^n
            - 8*binom(n,3)
            - 22*binom(n,5)
            + 16*binom(n,4)
            + 2*n
   Initial values: a(1)=a(2)=a(3)=0
*/

#include "../utils/big_pow.h"
#include "../utils/binom.h"

/* Return the nth term of A321269.
   n=1 is the first term in the OEIS offset.               */
seq_t a(idx_t n)
{
    if (n <= 3u)
        return 0;

    const seq_t ns = (seq_t)n;

    /* Powers of small integers – big_pow works for arbitrary exponents.  */
    seq_t pow4n  = big_pow((seq_t)4, (seq_t)n);           /* 4^n          */
    seq_t pow3n1 = big_pow((seq_t)3, (seq_t)(n - 1));     /* 3^(n-1)      */
    seq_t pow2n2 = big_pow((seq_t)2, (seq_t)(n - 2));     /* 2^(n-2)      */
    seq_t pow2n  = pow2n2 * (seq_t)4;                     /* 2^n          */

    /* binomial coefficients – binom accepts idx_t arguments.      */
    seq_t b2 = binom(n, 2);
    seq_t b3 = binom(n, 3);
    seq_t b4 = binom(n, 4);
    seq_t b5 = binom(n, 5);

    /* Assemble the formula.  All intermediate results fit in seq_t. */
    seq_t pos = pow4n
               + (seq_t)9 * b2 * pow2n2
               + (seq_t)16 * b4
               + (seq_t)2 * ns;

    seq_t neg = (seq_t)4 * ns * pow3n1
               + b3 * pow2n2
               + pow2n
               + (seq_t)8 * b3
               + (seq_t)22 * b5;

    return pos - neg;
}
