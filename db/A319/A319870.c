/*
 * A319870  a(n) = 6*5*4*3*2*1 + 12*11*10*9*8*7 + 18*17*16*15*14*13 + ... +
 *          (up to the n-th term).
 * https://oeis.org/A319870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{j=1}^{⌊(n‑1)/6⌋} ∏_{k=0}^{5} (6·j – k) +
 *            ∏_{k=0}^{(n‑1)%6} (6·⌈n/6⌉ – k)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 41, u64 n <= 965,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of cumulative sums of complete 6‑term blocks
       cum_sum[i] = sum of first i block products
    */
    static seq_t *cum_sum = NULL;   /* array of cumulative sums */
    static size_t len   = 0;        /* number of stored entries (cum_sum[0..len-1]) */
    static size_t cap   = 0;        /* allocated capacity of cum_sum */

    /* initialise cache on first call */
    if (cum_sum == NULL)
    {
        cap = 8;
        cum_sum = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 1;                    /* cum_sum[0] will be zero */
        cum_sum[0] = (seq_t)0;
    }

    /* block index (0‑based) and number of elements taken from that block (1‑6) */
    idx_t block   = (n - 1) / 6;
    idx_t m       = (n - 1) % 6 + 1;   /* length of the partial product */

    /* ensure cache contains the sum of all complete blocks before 'block' */
    while (len <= (size_t)block)
    {
        /* compute product of block (len-1) */
        idx_t blk_idx = len - 1;         /* 0‑based block index to compute */
        seq_t prod = (seq_t)1;
        idx_t start = 6 * (blk_idx + 1); /* highest number in this block */
        for (idx_t j = 0; j < 6; ++j)
            prod *= (seq_t)(start - j);

        /* grow the array if necessary */
        if (len == cap)
        {
            size_t newcap = (cap == 0) ? 8 : cap * 2;
            cum_sum = (seq_t *)realloc(cum_sum, newcap * sizeof(seq_t));
            cap = newcap;
        }

        /* store cumulative sum of first 'len' blocks */
        cum_sum[len] = cum_sum[len - 1] + prod;
        ++len;
    }

    /* previous full blocks sum */
    seq_t sum_prev = cum_sum[block];

    /* compute the partial product of the current block */
    seq_t partial = (seq_t)1;
    idx_t start   = 6 * (block + 1);
    for (idx_t i = 0; i < m; ++i)
        partial *= (seq_t)(start - i);

    return sum_prev + partial;
}
