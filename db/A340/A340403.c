/*
 * A340403  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where the
 *          removed edges are incident to different vertices in the 4-point
 *          set and none of the removed edges are...
 * https://oeis.org/A340403
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 343*15^(n-3) - 216*7^(n-3) + 4*3^(n-1) - 1 (offset 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Values below the offset are irrelevant */
    if (n < 4) return (seq_t)0;

    /* Powers */
    seq_t pow15 = big_pow((seq_t)15, (seq_t)(n - 3));
    seq_t pow7  = big_pow((seq_t)7 , (seq_t)(n - 3));
    seq_t pow3  = big_pow((seq_t)3 , (seq_t)(n - 1));

    /* Constants (as powers to avoid overflow on very small seq_t) */
    const seq_t c343 = big_pow((seq_t)7 , (seq_t)3);  /* 343 = 7^3 */
    const seq_t c216 = big_pow((seq_t)6 , (seq_t)3);  /* 216 = 6^3 */

    /* Terms of the formula */
    seq_t term1 = seq_mul(c343, pow15);            /* 343*15^(n-3) */
    seq_t term2 = seq_mul(c216, pow7 );            /* 216*7^(n-3)  */
    seq_t term3 = seq_mul((seq_t)4 , pow3 );       /* 4*3^(n-1)    */

    /* Assemble result using arithmetic operators (valid for all seq_t) */
    seq_t res = term1;
    res = res - term2;
    res = res + term3;
    res = res - (seq_t)1;

    return res;
}
