/*
 * A316828  Image of the Thue-Morse sequence A010060 under the morphism {1 ->
 *          1,2; 0 -> 0,2}.
 * https://oeis.org/A316828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is odd; otherwise a(n) = A010060(n/2) where
 *            A010060(k) is the Thue-Morse sequence (0 if parity of
 *            popcount(k) is even, 1 if odd)
 * Range:     u8 n <= 9748, u16 n <= 9748, u32 n <= 9748, u64 n <= 9748,
 *            u128 n <= 9748, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n & 1) /* odd n */
        return (seq_t)2;

    idx_t k = n >> 1;          /* n/2 */
    unsigned int parity = 0;   /* parity of popcount(k) */

    while (k) {
        parity ^= (unsigned int)(k & 1);
        k >>= 1;
    }

    return (seq_t)parity;
}
