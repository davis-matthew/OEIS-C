/*
 * A319888  a(n) = 5*4*3*2*1 - 10*9*8*7*6 + 15*14*13*12*11 - 20*19*18*17*16 +
 *          ... - (up to the n-th term).
 * https://oeis.org/A319888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   block_partial_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum_{i=1}^{k-1} (-1)^{i-1} * P_i
 *      + (-1)^{k-1} * P_k(r)
 * where k = floor((n-1)/5)+1,
 *      r = (n-1) % 5 + 1,
 *      P_i   = product_{j=0}^{4} (5*i - j),
 *      P_k(r)= product_{j=0}^{r-1} (5*k - j)   */
/* The sequence starts at index 1. */

#include <stdint.h>
#include "../utils/block_partial_product.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Assumes n >= 1. */
    if (n < 1)
        return 0;

    idx_t block = (n - 1) / 5 + 1;          /* 1-based block index */
    idx_t r     = (n - 1) % 5 + 1;          /* number of elements in current block */

    seq_t sum = 0;

    /* Sum contributions of complete previous blocks. */
    for (idx_t i = 1; i < block; ++i)
    {
        seq_t prod = block_partial_product(i, 5);    /* full block product */
        if ((i & 1) == 1)      /* odd block: positive */
            sum += prod;
        else
            sum -= prod;
    }

    /* Add contribution of the incomplete current block. */
    seq_t cur_prod = block_partial_product(block, r);
    if ((block & 1) == 1)   /* odd block: positive */
        sum += cur_prod;
    else
        sum -= cur_prod;

    return sum;
}
