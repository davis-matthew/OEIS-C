/*
 * A319550  a(n) = 1*2*3*4*5*6*7*8*9 - 10*11*12*13*14*15*16*17*18 +
 *          19*20*21*22*23*24*25*26*27 - ... + (up to n).
 * https://oeis.org/A319550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \sum_{blocks} (-1)^k * \prod_{i=1}^{r_k} (start_k + i)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 17, i64 n <= 143, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where each block has at most 9 consecutive integers and the sign alternates. */
seq_t a(idx_t n)
{
    seq_t sum  = 0;      /* accumulated alternating sum */
    seq_t prod = 1;      /* product of the current block */
    seq_t sign = 1;      /* +1 for first block, toggled each time */
    idx_t  counter = 0;  /* number of integers accumulated in current block */

    for (idx_t i = 1; i <= n; ++i)
    {
        prod *= (seq_t)i;      /* multiply by next integer */
        ++counter;             /* increment block counter */

        /* when a full block of 9 is complete or we reach the last integer, */
        /* add its contribution and start a new block */
        if (counter == 9 || i == n)
        {
            sum += sign * prod;
            sign = -sign;      /* toggle block sign */
            prod = 1;          /* reset product for next block */
            counter = 0;
        }
    }

    return sum;
}
