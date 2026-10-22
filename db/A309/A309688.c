/*
 * A309688  Sum of the odd parts appearing among the second largest parts of
 *          the partitions of n into 3 parts.
 * https://oeis.org/A309688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} i * (i
 *            mod 2)
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k = n / 3;                 /* maximum possible second largest part     */
    for (idx_t j = 1; j <= k; ++j)   /* iterate over the second largest part     */
    {
        idx_t i_max = (n - j) / 2;   /* third part is n - i - j, must be >= i   */
        for (idx_t i = j; i <= i_max; ++i)
        {
            if (i & 1)               /* only odd i contribute (odd part)         */
                sum += (seq_t)i;
        }
    }
    return sum;
}
