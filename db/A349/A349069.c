/*
 * A349069  a(n) = H(n, 3*n), where H(n,x) is n-th Hermite polynomial.
 * https://oeis.org/A349069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H(n,3*n) (physicists Hermite polynomial)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle first term explicitly – avoids division by zero in the loop. */
    if (n == 0) return (seq_t)1;

    /* x = 3*n, base = 2*x = 6*n  */
    seq_t base = (seq_t)6 * (seq_t)n;

    /* Initial term: T0 = base^n = (6*n)^n  */
    seq_t term   = big_pow(base, (seq_t)n);
    seq_t result = term;

    /* Sign of the next term: alternating, start with negative. */
    int sign = -1;

    /* Number of iterations equals floor(n/2). Each iteration computes
       the next term in the binomial expansion:
           T_{k+1} = T_k * (n-2k)(n-2k-1) / ((k+1)*(6n)^2)
    */
    idx_t max_k = n / 2;
    for (idx_t k = 0; k < max_k; ++k)
    {
        /* Divide by (6n)^2 – safe to divide by base twice to avoid
           overflow in base*base for small seq_t types. */
        term = term / base;
        term = term / base;

        /* Multiply by (n-2k)(n-2k-1) */
        seq_t mult = (seq_t)(n - 2 * k) * (seq_t)(n - 2 * k - 1);
        term = term * mult;

        /* Divide by (k+1) */
        term = term / ((seq_t)(k + 1));

        /* Apply alternating sign and accumulate. */
        if (sign > 0)
            result += term;
        else
            result -= term;

        /* Flip sign for next round. */
        sign = -sign;
    }

    return result;
}
