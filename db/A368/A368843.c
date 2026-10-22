/*
 * A368843  a(n) gives the number of triples of equally spaced 1's in the
 *          binary expansion of n.
 * https://oeis.org/A368843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of triples of equally spaced 1's in the binary
 *            expansion of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* a(n): number of triples of equally spaced 1's in the binary representation of n */
seq_t a(idx_t n)
{
    /* collect positions of set bits in ascending order */
    size_t pos[64];
    size_t bc = 0;
    idx_t temp = n;
    for (size_t bit = 0; temp; ++bit) {
        if (temp & 1U)
            pos[bc++] = bit;
        temp >>= 1;
    }

    /* count triples i, j, k such that i, j, k are equally spaced and all bits are 1 */
    seq_t count = 0;
    for (size_t a = 0; a < bc; ++a) {
        for (size_t b = a + 1; b < bc; ++b) {
            size_t d  = pos[b] - pos[a];
            size_t k  = pos[a] + 2 * d;
            if (k < 64 && ((n >> k) & 1U))
                count++;
        }
    }

    return count;
}
