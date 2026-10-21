/*
 * A246594  a(n)=n for n <= 2; for n >= 3, a(n) = largest number that can be
 *          obtained by swapping two adjacent bits in the binary expansion of
 *          n.
 * https://oeis.org/A246594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for n <= 2; otherwise, the largest number obtainable by
 *            swapping the first adjacent “01” pair to “10” in the binary
 *            expansion of n (ignoring leading zeros)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Trivial cases are handled directly. */
    if (n <= 2)
        return (seq_t)n;

    seq_t x = (seq_t)n;

    /* Find the index of the most significant set bit (0‑based). */
    unsigned msb = bitlen_seq_t(x) - 1;

    /* Scan from msb down to 1 looking for the first “01” pattern
       and swap it to “10”. */
    for (int j = msb; j > 0; --j)
    {
        if ( ((x >> j) & 1) == 0 && ((x >> (j - 1)) & 1) == 1 )
        {
            /* Turn 01 -> 10 at positions j (high) and j-1 (low). */
            x = (x | ((seq_t)1 << j)) & ~(((seq_t)1 << (j - 1)));
            return x;
        }
    }

    /* No occurrence of “01” was found; return n unchanged. */
    return x;
}
