/*
 * A368857  a(n) gives the maximum number of equally spaced equal digits in
 *          the binary expansion of n (without leading zeros).
 * https://oeis.org/A368857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A368857: the maximum number of equally spaced equal
   digits in the binary expansion of n (without leading zeros). */

/* The algorithm inspects all arithmetic progressions of bit positions
   in the binary representation of n and returns the largest length
   found.  It runs in O(k^3) time, where k = ⌊log2(n)⌋+1 ≤ 64. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return (seq_t)0;

    /* compute the length of the binary representation */
    idx_t temp = n;
    idx_t len = 0;
    do {
        temp >>= 1;
        ++len;
    } while (temp);

    seq_t best = 1;                /* at least one equal digit exists */

    for (idx_t bit = 0; bit <= 1; ++bit) {
        for (idx_t start = 0; start < len; ++start) {
            if (((n >> start) & 1ULL) != bit)
                continue;          /* skip start positions without the bit */

            for (idx_t step = 1; step < len; ++step) {
                idx_t pos  = start;
                idx_t cnt  = 0;

                while (pos < len && ((n >> pos) & 1ULL) == bit) {
                    ++cnt;
                    pos += step;
                }

                if (cnt > best)
                    best = cnt;
            }
        }
    }

    return best;
}
