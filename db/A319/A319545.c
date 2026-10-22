/*
 * A319545  a(n) = 1*2*3*4*5 - 6*7*8*9*10 + 11*12*13*14*15 - ... + (up to n).
 * https://oeis.org/A319545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k=0}^{⌊(n-1)/5⌋} (-1)^k * product of the integers
 *            5k+1 … min(5k+5, n)
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 84, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the alternating block‑product sequence A319545.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulator for the sum of blocks   */
    idx_t  start  = 1;         /* start of the current block          */
    unsigned int group = 0;    /* block index (0‑based)               */

    while (start <= n)
    {
        /* end of the current block (not exceeding n) */
        idx_t end = start + 5 - 1;
        if (end > n) end = n;

        /* product of the numbers in the current block */
        seq_t prod = 1;
        for (idx_t i = start; i <= end; ++i)
        {
            prod *= (seq_t)i;
        }

        /* add or subtract the product depending on the block index */
        if (group & 1)
            result -= prod;   /* odd block: negative sign */
        else
            result += prod;   /* even block: positive sign */

        /* move to the next block */
        start = end + 1;
        ++group;
    }

    return result;
}
