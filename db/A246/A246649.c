/*
 * A246649  Integers of the form (2^(k+1) - 1)/(2*k + 1).
 * https://oeis.org/A246649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(k+1)-1)/(2*k+1) where k is the n-th index such that
 *            the quotient is an integer
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t k = 0; ; ++k)
    {
        seq_t denom = (seq_t)(2*k + 1);
        seq_t numer = pow2(k + 1) - (seq_t)1;
        seq_t q = numer / denom;
        if (q * denom == numer) {
            ++found;
            if (found == n) {
                return q;
            }
        }
    }
}
