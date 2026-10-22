/*
 * A337005  Leech's order 13 uniform cyclic squarefree word.
 * https://oeis.org/A337005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of t(d) for each base-13 digit of n) mod 3, where t
 *            = {0,1,2,1,0,2,1,2,0,1,2,1,0}
 * Range:     u8 n <= 6590, u16 n <= 6590, u32 n <= 6590, u64 n <= 6590,
 *            u128 n <= 6590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* t[d] encodes the expansion term for digit d (0 ≤ d < 13). */
    static const uint8_t t[13] = {
        0, 1, 2, 1, 0, 2, 1, 2, 0, 1, 2, 1, 0
    };

    seq_t sum = 0;
    idx_t m = n;

    while (m > 0) {
        unsigned int d = (unsigned int)(m % 13ULL);
        sum += t[d];
        m /= 13ULL;
    }

    return sum % 3;
}
