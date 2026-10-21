/*
 * A277563  Fifth column of Euler's difference table in A068106.
 * https://oeis.org/A277563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-4} (-1)^j * binomial(n-4,j) * (n-j)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1)=0, a(2)=0, a(3)=0, a(4)=24                           */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n < 4) return (seq_t)0;
    if (n == 4) return (seq_t)24;

    /* Compute n!   */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Sum of the terms; term j=0 is already included */
    seq_t sum  = fact;          /* j = 0 */
    seq_t binom = (seq_t)1;     /* binomial(n-4,0) */
    int   sign  = -1;           /* sign for j = 1   */

    /* Loop over j = 1 .. n-4 */
    for (idx_t j = (idx_t)1; j <= n - (idx_t)4; ++j)
    {
        /* Update binomial coefficient: binom = binom * (n-4-(j-1)) / j */
        binom *= (seq_t)(n - (idx_t)4 - (j - (idx_t)1));
        binom  = binom / (seq_t)j;

        /* Update factorial part: divide by (n - j + 1) */
        fact /= (seq_t)(n - j + (idx_t)1);

        /* Current term */
        seq_t term = binom * fact;

        /* Add with the appropriate sign */
        if (sign > 0)
            sum += term;
        else
            sum -= term;

        /* Flip sign for next term */
        sign = -sign;
    }

    return sum;
}
