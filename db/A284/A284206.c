/*
 * A284206  Tenth column of Euler's difference table in A068106.
 * https://oeis.org/A284206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A284206 – Tenth column of Euler's difference table
 * Formula: a(n) = Sum_{j=0..n-9} (-1)^j * binomial(n-9,j) * (n-j)!
 * Offset: 1
 */

#include <stddef.h>

/* local factorial (use shared helper if available) */
static seq_t factorial(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n)
{
    /* values for n < 9 are zero */
    if (n < (idx_t)9)
        return (seq_t)0;

    idx_t m = n - (idx_t)9;           /* n-9  (>= 0)  */
    seq_t sum = (seq_t)0;
    seq_t term = factorial(n);       /* term for j = 0   */
    int sign = 1;                    /* (-1)^j  */

    for (idx_t j = (idx_t)0; j <= m; ++j)
    {
        /* add current term with its sign */
        if (sign > 0)
            sum += term;
        else
            sum -= term;

        if (j == m)
            break;                   /* finished all terms */

        /* compute term_{j+1} from term_j
         * term_{j+1} = term_j / (n-j) * (m-j) / (j+1)
         * All divisions are exact on integer domain.           */
        term = term / (seq_t)(n - j);
        term = term * (seq_t)(m - j);
        term = term / (seq_t)(j + (idx_t)1);

        sign = -sign;                /* toggle sign for next j */
    }
    return sum;
}
