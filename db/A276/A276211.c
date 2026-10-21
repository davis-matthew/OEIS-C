/*
 * A276211  Position of n^s in the joint ranking of {h^r} and {k^s}, where r =
 *          sqrt(2), s = sqrt(5), h > 1, k > 1.
 * https://oeis.org/A276211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n+1)^(sqrt(5)/sqrt(2))) - 1
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses long double arithmetic to evaluate the
 * irrational exponent accurately for the domain of the tests.   */
#include <math.h>

seq_t a(idx_t n)
{
    /* constant exponent sqrt(5/2) */
    static const long double exp_const = sqrtl(5.0L) / sqrtl(2.0L);

    /* compute (n+1)^exp_const using long double */
    long double base = (long double)n + 1.0L;
    long double val  = powl(base, exp_const);

    /* floor the result and cast to integer types */
    idx_t f = (idx_t)floorl(val);

    /* final answer: a(n) = n + floor((n+1)^exp_const) - 1   */
    return (seq_t)n + (seq_t)f - 1;
}
