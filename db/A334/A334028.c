/*
 * A334028  Number of distinct parts in the n-th composition in standard
 *          order.
 * https://oeis.org/A334028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of distinct parts in the n‑th composition in
   standard order (OEIS A334028).  The composition is derived from the
   binary expansion of n: the positions of the 1 bits (counted from 1)
   give the part–differences.  This routine counts distinct difference
   values. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    bool seen[65] = {0};            /* indices 1..64 hold presence flag */
    idx_t pos = 1;
    idx_t prev = 0;
    bool first = true;

    while (n)
    {
        if (n & 1)
        {
            idx_t diff;
            if (first)
            {
                diff = pos;
                first = false;
                prev = pos;
            }
            else
            {
                diff = pos - prev;
                prev = pos;
            }
            if (diff <= 64)
                seen[diff] = true;
        }
        n >>= 1;
        pos++;
    }

    int count = 0;
    for (int d = 1; d <= 64; ++d)
        if (seen[d])
            count++;

    return (seq_t)count;
}
