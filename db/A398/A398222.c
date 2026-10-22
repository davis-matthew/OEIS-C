/*
 * A398222  a(n) = Sum_{j=1..n} c(n,j)^2 * (2j-1)! * (2j-1), where c(n,j) =
 *          (2n)!/((n-j)! * (2j)! * 2^(n-j)).
 * https://oeis.org/A398222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n} c(n,j)^2 * (2j-1)! * (2j-1), where c(n,j) =
 *            (2n)! / ((n-j)! * (2j)! * 2^(n-j))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; 0 is undefined, return 0 */

    /* compute factorial(2n) and factorial(n-1) */
    seq_t fact = 1;           /* running factorial of i */
    seq_t fact_nm1 = 1;       /* factorial(n-1), default 1 for n=1 */

    for (idx_t i = 1; i <= 2 * n; ++i) {
        fact *= (seq_t)i;
        if (i == n - 1 && n > 0)
            fact_nm1 = fact;
    }
    if (n == 1)            /* factorial(0) = 1 */
        fact_nm1 = 1;

    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* c(n,1) = (2n)! / ((n-1)! * 2^n) */
    seq_t denom_c1 = fact_nm1 * pow2;
    seq_t c_curr = fact / denom_c1;   /* current c(n,j) value */

    seq_t sum = 0;

    /* factorial(2j-1) maintained iteratively */
    seq_t fact_odd = 1;   /* factorial of 1 */

    for (idx_t j = 1; j <= n; ++j) {
        /* term = c_j^2 * (2j-1)! * (2j-1) */
        seq_t term = c_curr * c_curr;
        term = term * fact_odd;
        term = term * (seq_t)(2 * j - 1);
        sum += term;

        if (j < n) {
            /* update c to next j: c(n,j+1) = c(n,j) * 2*(n-j) / ((2j+1)(2j+2)) */
            seq_t num = (seq_t)(2 * (n - j));
            seq_t den = (seq_t)(2 * j + 1) * (seq_t)(2 * j + 2);
            c_curr = (c_curr * num) / den;

            /* update factorial(2j+1) from factorial(2j-1) */
            fact_odd = fact_odd * (seq_t)(2 * j);
            fact_odd = fact_odd * (seq_t)(2 * j + 1);
        }
    }
    return sum;
}
