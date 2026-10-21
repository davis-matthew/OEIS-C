/*
 * A258059  Let n = Sum_{i=0..k} d_i*4^i be the base-4 expansion of n, with 0
 *          <= d_i < 4. Then a(n) = minimal i such that d_i is not 1, or k+1
 *          if there is no such i.
 * https://oeis.org/A258059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = minimal i such that the i‑th base‑4 digit of n is not 1;
 *            * if all digits are 1 then a(n) = (number of digits of n in base
 *            4). * Recurrence: a(1)=1; a(4*n+1)=a(n)+1; a(4*n+j)=0 for
 *            j=0,2,3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case when n == 0, although it's outside
     * the defined range.  Returning 0 is harmless. */
    if (n == 0)
        return 0;

    idx_t m = n;
    seq_t i = 0;

    while (m > 0)
    {
        idx_t digit = m & 3;             /* m % 4 */
        if (digit != 1)
            return i;
        m >>= 2;                         /* divide by 4 */
        ++i;
    }
    return i;                             /* all digits were 1 */
}
