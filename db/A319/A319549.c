/*
 * A319549  a(n) = 1*2*3*4*5*6*7*8 - 9*10*11*12*13*14*15*16 +
 *          17*18*19*20*21*22*23*24 - ... + (up to n).
 * https://oeis.org/A319549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 23, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0}^{⌈n/8⌉-1} (−1)^k * Π_{j=8k+1}^{min(8k+8, n)} j
 *   (sum of products of consecutive blocks of size 8, alternating signs,
 *   stopping at n.)
 */
 
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t block = 0; ; ++block)
    {
        idx_t start = block * 8ull + 1;          /* block start index */
        if (start > n) break;                   /* no more blocks to process */

        idx_t end = (n < start + 7ull) ? n : start + 7ull;  /* last included in this block */

        seq_t prod = 1;
        for (idx_t x = start; x <= end; ++x)
            prod *= (seq_t)x;                     /* product of numbers in this block */

        if ((block & 1) == 0)  /* even block index: sign +1 */
            sum += prod;
        else
            sum -= prod;
    }
    return sum;
}
