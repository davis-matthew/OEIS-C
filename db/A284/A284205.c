/*
 * A284205  Ninth column of Euler's difference table in A068106.
 * https://oeis.org/A284205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-8} (-1)^j * binomial(n-8,j) * (n-j)!
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    idx_t m = n - 8;                 /* number of terms in the sum */
    seq_t fac = factorial(m);        /* (n-8)! */

    /* product of the 8 consecutive numbers (n-7)*(n-6)*...*n */
    seq_t P = 1;
    for (idx_t i = 1; i <= 8; ++i)
        P *= (seq_t)(n - 8 + i);

    /* term for j = 0 */
    seq_t sum = fac * P;             /* k = 0, sign + */
    seq_t k_fact = 1;                /* factorial of k */

    for (idx_t k = 1; k <= m; ++k)
    {
        /* Update P from P_{k-1} to P_k:
           P_k = P_{k-1} * (n-8-k+1) / (n-k+1)   */
        seq_t num = (seq_t)(n - 8 - k + 1);   /* lower new term   */
        seq_t den = (seq_t)(n - k + 1);       /* upper old term   */
        P = P * num;
        P = P / den;

        /* Update k! */
        k_fact *= (seq_t)k;

        /* Compute term: fac * P / k! */
        seq_t term = fac * P / k_fact;

        /* Apply sign */
        if (k & 1)  /* odd k -> subtract */
            sum -= term;
        else
            sum += term;
    }

    return sum;
}
