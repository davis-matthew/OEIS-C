/*
 * A375966  Powers of 3 alternating with powers of 4.
 * https://oeis.org/A375966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 3^k, a(2k+1) = 4^k
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Use unsigned 64–bit arithmetic for n; shift right by one
     * to divide by 2 without underflowing when n is 0. */
    if ((n & 1ULL) == 0) {          /* n is even  →  k = n/2 */
        return big_pow((seq_t)3, (seq_t)(n >> 1));
    } else {                        /* n is odd   →  k = (n-1)/2 */
        return big_pow((seq_t)4, (seq_t)(n >> 1));
    }
}
