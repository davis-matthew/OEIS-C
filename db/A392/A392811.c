/*
 * A392811  Number of transfer systems on the horizontal join lattice [n]*[n].
 * https://oeis.org/A392811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Catalan(n)^2 +
 *            Catalan(n-1)*(Catalan(n+1)-2*Catalan(n))) for n > 0; a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t b_n  = catalan(n);
    seq_t b_nm1 = catalan(n - 1);
    seq_t b_np1 = catalan(n + 1);

    seq_t two = (seq_t)2;
    seq_t term1 = b_n * b_n;                     /* Catalan(n)^2 */
    seq_t term2 = b_np1 - (two * b_n);           /* Catalan(n+1) - 2*Catalan(n) */
    seq_t term3 = b_nm1 * term2;                 /* Catalan(n-1) * ( ... ) */
    seq_t sum   = term1 + term3;

    return sum * two;
}
