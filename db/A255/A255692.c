/*
 * A255692  Convert n to base 7, move the most significant digit to the least
 *          significant one and convert back to base 10.
 * https://oeis.org/A255692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = ((n mod pow7) * 7 + (n / pow7)), where pow7 is the
 *            largest power of 7 not exceeding n (i.e., 7^k <= n < 7^(k+1))
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the largest power of 7 <= n. */
    unsigned __int128 p = 1;
    while (p * 7 <= n) {
        p *= 7;
    }
    uint64_t power = (uint64_t)p;       /* power = 7^k */
    uint64_t msd   = n / power;        /* most significant digit in base 7 */
    uint64_t rest  = n % power;        /* remaining lower digits */

    /* Compute result: move msd to least significant place. */
    return (seq_t)rest * 7 + (seq_t)msd;
}
