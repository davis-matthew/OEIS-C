/*
 * A319546  a(n) = 1*2*3*4*5*6 - 7*8*9*10*11*12 + 13*14*15*16*17*18 - ... +
 *          (up to n).
 * https://oeis.org/A319546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \sum_{k=0}^{\lfloor (n-1)/6 \rfloor} (-1)^k *
 *            \prod_{j=0}^{\min(5,\,n-6k-1)} (6k+1 + j) * * That is, split the
 *            positive integers into blocks of 6. * For each full block add
 *            the product of its 6 numbers if the block * index is even,
 *            subtract it if odd. For the final (possibly * incomplete)...
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by contract */
    insert:
    /* Number of complete blocks is floor((n-1)/6).  */
    idx_t last_block = (n - 1) / 6;           /* block indices: 0 .. last_block */
    seq_t result = 0;

    for (idx_t blk = 0; blk <= last_block; ++blk) {
        idx_t start = 6 * blk + 1;
        idx_t end;
        /* If this is the last block and n < start+6, we take only up to n. */
        if (blk == last_block && n % 6 != 0)
            end = n;
        else
            end = start + 5;                  /* full block */
        /* Compute the product of numbers start .. end */
        seq_t prod = 1;
        for (idx_t x = start; x <= end; ++x)
            prod *= (seq_t)x;
        /* Determine sign: + for even block, - for odd block. */
        if (blk & 1)
            result -= prod;
        else
            result += prod;
    }

    return result;
}
