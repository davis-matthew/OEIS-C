/*
 * A372845  a(n) is the number of parking functions of order n for which the
 *          fifth spot is lucky.
 * https://oeis.org/A372845
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/5)*(n+1)^(n-1) - (1/30)*(118*n^3 - 531*n^2 + 659*n -
 *            192)*(n-4)^(n-4)
 * Range:     u8 none, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Accept n >= 5; for n < 5 the value is undefined but we return 0 */
    if (n < 5)
        return 0;

    seq_t tn   = (seq_t)n;
    seq_t tnp1 = tn + 1;          /* n+1 */
    seq_t tnm4 = (tn - 4);        /* n-4 */

    /* Compute the two powers */
    seq_t pow1 = big_pow(tnp1, (seq_t)(n - 1));        /* (n+1)^(n-1) */
    seq_t pow2 = big_pow(tnm4, (seq_t)(n - 4));        /* (n-4)^(n-4) */

    /* Polynomial part: 118*n^3 - 531*n^2 + 659*n - 192 */
    seq_t poly = 118 * tn * tn * tn
               - 531 * tn * tn
               + 659 * tn
               - 192;

    /* Compute the two terms with integer division */
    seq_t term1 = (3 * pow1) / 5;      /* (3/5)*(n+1)^(n-1) */
    seq_t term2 = (poly * pow2) / 30;  /* (1/30)*poly*(n-4)^(n-4) */

    return term1 - term2;
}
