/*
 * A357580  a(n) = ((1 + sqrt(n))^n - (1 - sqrt(n))^n)/(2*n*sqrt(n)).
 * https://oeis.org/A357580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..⌊n/2⌋} n^(j-1) * binomial(n, 2*j+1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n‑th term of OEIS A357580.
   The index n is 1‑based; a(1) = 1.  */
seq_t a(idx_t n)
{
    if (n == 0)                /* defensive guard; offset is 1 */
        return 0;

    seq_t sum   = (seq_t)1;     /* j = 0 term: 1 */
    seq_t curC  = (seq_t)n;     /* C(n,1)                     */
    idx_t  k    = (idx_t)1;     /* current binomial parameter  */
    seq_t  power = (seq_t)1;    /* n^(j-1) for j = 1           */

    /* Iterate j = 1 .. ⌊n/2⌋, accumulating the sum. */
    for (idx_t j = (idx_t)1; j <= n / (idx_t)2; ++j)
    {
        /* Compute C(n, k+2) from C(n, k) by the identity:
           C(n, k+2) = C(n, k) * (n - k) * (n - k - 1) / ((k + 1)(k + 2)) */
        seq_t num1  = (seq_t)(n - k);
        seq_t num2  = (seq_t)(n - k - (idx_t)1);
        seq_t numer = curC * num1;
        numer = numer * num2;
        seq_t denom = (seq_t)(k + (idx_t)1);
        denom = denom * (k + (idx_t)2);
        curC = numer / denom;   /* exact division */

        k += (idx_t)2;           /* advance k to the next odd index */

        /* Add the current term: n^(j-1) * C(n, 2j+1) */
        seq_t term = power * curC;
        sum += term;

        /* Update power for the next j */
        power = power * (seq_t)n;
    }

    return sum;
}
