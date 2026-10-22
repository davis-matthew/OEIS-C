/*
 * A393412  Sum of the largest parts in the partitions of n into 3 parts whose
 *          middle part is odd.
 * https://oeis.org/A393412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋} Σ_{k=j..⌊(n−j)/2⌋} (n−j−k) · (k mod 2)
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* No partitions when n < 3; loop will not execute */
    for (idx_t j = 1; j <= n / 3; ++j)
    {
        idx_t maxk = (n - j) / 2;               /* floor((n − j)/2) */
        for (idx_t k = j; k <= maxk; ++k)
        {
            if (k & 1)                          /* k is odd */
            {
                sum += (seq_t)(n - j - k);      /* add the largest part */
            }
        }
    }
    return sum;
}
