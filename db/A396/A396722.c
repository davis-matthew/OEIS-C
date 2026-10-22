/*
 * A396722  a(n) = (4^n/2) * p(n, 5/2), where p(n,x) is the Lagrange
 *          interpolating polynomial through (1,0) and (k,1) for k=2..n+1.
 * https://oeis.org/A396722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (4^n/2) * p(n, 5/2) where p(n,x) is the Lagrange
 *            interpolating polynomial through (1,0) and (k,1) for k=2..n+1.
 *            Recurrence (conjectured): n*a(n) + 2*(-4*n+5)*a(n-1) +
 *            8*(2*n-5)*a(n-2) = 0 with a(0) = 0, a(1) = 3. The implementation
 *            below evaluates this recurrence iteratively, using only in...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)3;

    seq_t a0 = (seq_t)0;   /* a(n-2) */
    seq_t a1 = (seq_t)3;   /* a(n-1) */
    seq_t an = a1;         /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        int64_t im = (int64_t)i;
        /* Coefficients derived from the recurrence:
           8*i-10  multiplies a(n-1)
           -16*i+40 multiplies a(n-2)           */
        seq_t term1 = (seq_t)(8 * im - 10) * a1;
        seq_t term2 = (seq_t)(-16 * im + 40) * a0;
        seq_t num   = term1 + term2;
        an           = num / (seq_t)i;

        /* Shift for next iteration */
        a0 = a1;
        a1 = an;
    }
    return an;
}
