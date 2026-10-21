/*
 * A271385  a(n) = Product_{k=0..floor((n - 1)/2)} (n - 2*k)^(n - 2*k).
 * https://oeis.org/A271385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n*(n-2)^(n-2)*...*(1)^1 for odd n, a(n) =
 *            n^n*(n-2)^(n-2)*...*(2)^2 for even n; a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t i = n; i > 0; ) {
        /* compute i^i */
        seq_t term = pow_int((seq_t)i, i);
        result *= term;
        /* stop when the next value would be <= 0 for unsigned i */
        if (i <= 2)
            break;
        i -= 2;
    }
    return result;
}
