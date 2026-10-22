/*
 * A319874  a(n) = 10*9*8*7*6*5*4*3*2*1 + 20*19*18*17*16*15*14*13*12*11 + ...
 *          + (up to the n-th term).
 * https://oeis.org/A319874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 89, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A319874 – cumulative sums of partial products of descending
    blocks of length 10.
    First term a(1)=10.
    For each block that starts at a multiple of 10, the increment at
    step n is the difference between the product of the numbers added
    in the current block and the product before.   This reproduces
    the OEIS sequence. */

seq_t a(idx_t n)
{
    const idx_t K = 10;                  /* block length */

    seq_t sum = 0;                       /* resulting a(n)  */
    seq_t block_start = (seq_t)K;        /* starting number of current block */
    seq_t partial_product = 0;           /* product of numbers added so far in block */
    idx_t j = 0;                         /* how many numbers added in current block */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t delta;

        if (j == 0)                     /* first number of a new block */
        {
            partial_product = block_start;
            delta = partial_product;
        }
        else                            /* subsequent numbers in the same block */
        {
            seq_t next_number = block_start - (seq_t)j;   /* next descending value     */
            seq_t new_product = partial_product * next_number;
            delta = new_product - partial_product;
            partial_product = new_product;
        }

        sum += delta;                   /* accumulate result       */

        ++j;                            /* one more number taken in block */

        if (j == K)                     /* block finished           */
        {
            j = 0;
            block_start += (seq_t)K;    /* move to next block    */
        }
    }

    return sum;
}
