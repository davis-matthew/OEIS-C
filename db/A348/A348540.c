/*
 * A348540  Number of partitions of n into 3 parts whose smallest part divides
 *          the largest part.
 * https://oeis.org/A348540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts whose smallest
 *            part divides the largest part
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 3) return 0;   /* no 3-part partition possible */

    for (idx_t a = 1; a <= n / 3; ++a)               /* a = smallest part */
    {
        idx_t max_b = (n - a) / 2;                   /* b <= c, b >= a */
        for (idx_t b = a; b <= max_b; ++b)           /* b = middle part */
        {
            idx_t c = n - a - b;                     /* c = largest part */
            /* c >= b always holds because b <= (n-a)/2 */
            if ((c % a) == 0)                        /* smallest divides largest */
            {
                ++count;
            }
        }
    }
    return count;
}
