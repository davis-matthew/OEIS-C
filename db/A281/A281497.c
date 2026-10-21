/*
 * A281497  Write n in binary reflected Gray code and sum the positions where
 *          there is a '1' followed immediately to the left by a '0', counting
 *          the rightmost digit as position 1.
 * https://oeis.org/A281497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A049502(A003188(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of positions (counted from the right starting at 1) where
   a binary reflected Gray code contains a '1' immediately preceded by a
   '0'.  For a given n≥1: a(n) = Σ i for which  (g[i]==1 && g[i+1]==0),
   where g is the Gray code of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Gray code of n */
    idx_t g = n ^ (n >> 1);

    /* Determine bit-length of g (highest set bit position) */
    idx_t bitlen = 0;
    idx_t t = g;
    while (t > 0) {
        t >>= 1;
        bitlen++;
    }

    seq_t sum = 0;
    /* Scan positions 1 .. bitlen-1 (highest bit has no left neighbour) */
    for (idx_t i = 1; i < bitlen; ++i) {
        /* bit at position i (1-indexed from LSB) */
        idx_t bit_i   = (g >> (i - 1)) & 1;
        /* bit immediately left (position i+1) */
        idx_t bit_ip1 = (g >> i) & 1;

        if (bit_i && !bit_ip1) {
            sum += (seq_t)i;
        }
    }

    return sum;
}
