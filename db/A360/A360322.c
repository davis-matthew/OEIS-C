/*
 * A360322  a(n) = Sum_{k=0..n} (-5)^(n-k) * binomial(n-1,n-k) *
 *          binomial(2*k,k).
 * https://oeis.org/A360322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k=0..n} (-5)^(n-k) * binomial(n-1,n-k) *
 *            binomial(2*k,k). Recurrence: for n>=2, n * a(n) = 2 * (-3*n + 4)
 *            * a(n-1) - 5 * (n-2) * a(n-2). Initial values: a(0) = 1, a(1) =
 *            2. This function uses the recurrence and requires only O(1)
 *            extra space
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* a0 = a(n-2), a1 = a(n-1) initially a0=a(0), a1=a(1). */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute the coefficient -3*i + 4 as a signed seq_t. */
        seq_t coeff = ((seq_t)-3) * (seq_t)i + (seq_t)4;
        /* Numerator: 2*coeff*a1 - 5*(i-2)*a0. */
        seq_t term1 = (seq_t)2 * coeff * a1;
        seq_t term2 = (seq_t)5 * (seq_t)(i - 2) * a0;
        seq_t num = term1 - term2;
        /* Divide by i to get a(i).  The division is exact by the recurrence. */
        seq_t ai = num / (seq_t)i;

        /* Shift for next iteration: a0 <- a1, a1 <- ai. */
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
