/*
 * A319544  a(n) = 1*2*3*4 - 5*6*7*8 + 9*10*11*12 - 13*14*15*16 + ... - (up to
 *          n).
 * https://oeis.org/A319544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{b=0}^{⌊(n-1)/4⌋} (-1)^b * Π_{k=0}^{m_b-1} (4b+1+k),
 *            where m_b = min(4, n-4b). This sums the products of consecutive
 *            integers in blocks of four, alternating signs between blocks
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ) {
        /* block index starting at 0, determines sign */
        idx_t block_index = (i - 1) / 4;
        seq_t sign = (block_index & 1) ? -1 : 1;

        /* end of this block (cannot exceed n) */
        idx_t block_end = i + 3;
        if (block_end > n) block_end = n;

        /* product of numbers in this block */
        seq_t prod = 1;
        for (idx_t k = i; k <= block_end; ++k) {
            prod *= (seq_t)k;
        }

        sum += sign * prod;
        i = block_end + 1;   /* move to next block start */
    }
    return sum;
}
