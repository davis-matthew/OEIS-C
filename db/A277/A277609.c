/*
 * A277609  Fourth column of Euler's difference table in A068106. It is 6
 *          times the sequence A000261.
 * https://oeis.org/A277609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-3} (-1)^j*binomial(n-3,j)*(n-j)! (a(1)=0,
 *            a(2)=0, a(3)=6)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n<1, but return 0 */

    if (n == 1 || n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)6;

    idx_t k = n - 3;                    /* number of terms in the sum */
    seq_t ans  = (seq_t)0;
    seq_t fact = factorial(n);          /* (n-j)! for j = 0 */

    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t term = binom(k, j) * fact;
        if ((j & 1ULL) == 0)
            ans += term;                /* even j: add */
        else
            ans -= term;                /* odd j: subtract */

        if (j < k)                      /* prepare factorial for next j */
            fact /= (seq_t)(n - j);      /* (n-(j+1))! = (n-j)! / (n-j) */
    }

    return ans;
}
