/*
 * A246574  a(n) = 2*(n-1)*Catalan(n).
 * https://oeis.org/A246574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*Catalan(n) = 4*binomial(2*n-1, n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* For safety, though n >= 1 per offset */

    seq_t cat = catalan(n);           /* Catalan number C_n */
    seq_t n1  = (seq_t)(n - 1);       /* Convert (n-1) to seq_t */
    return 2 * n1 * cat;              /* 2*(n-1)*C_n */
}
