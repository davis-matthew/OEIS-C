/*
 * A316867  Number of times 6 appears in decimal expansion of n.
 * https://oeis.org/A316867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = (number of occurrences of digit 6 in decimal expansion of
 *            n)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t temp   = n;            /* use unsigned arithmetic on the index        */
    while (temp > 0)            /* loop over decimal digits                    */
    {
        if (temp % 10 == 6)     /* digit is 6?                                 */
            count += 1;         /* increment the count                         */
        temp /= 10;             /* move to next digit                          */
    }
    return count;               /* zero has no 6, the loop is skipped          */
}
