/*
 * A275434  Sum of the degrees of asymmetry of all compositions of n.
 * https://oeis.org/A275434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(n)=((3*n-2)*2^n + (n%2?16:-16))/36 for n>=2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that produces 2^n */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* zero and one give zero */
    if (n < 2U)
        return (seq_t)0;

    /* Compute 2^n using the provided helper */
    seq_t p2 = pow2(n);

    /* Compute (3*n-2)*2^n */
    idx_t nn = n;
    seq_t mult = (seq_t)((3 * nn) - 2);
    seq_t term1 = mult * p2;

    /* Add or subtract 16 according to the parity of n */
    if (nn & 1U)          /* n odd  -> +16 */
    {
        term1 += (seq_t)16;
    }
    else                  /* n even -> -16 */
    {
        term1 -= (seq_t)16;
    }

    /* Divide by 36 to obtain the result */
    return term1 / (seq_t)36;
}
