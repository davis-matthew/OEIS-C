/*
 * A345877  a(1) = 1, a(n) = a(n-1)/2 if a(n-1) is even, otherwise a(n) = n -
 *          a(n-1).
 * https://oeis.org/A345877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=a(n-1)/2 if a(n-1) even, else n-a(n-1)
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                     /* a(1) */
    if (n <= 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((val & 1) == 0)            /* previous value is even */
            val = val / 2;
        else
            val = (seq_t)i - val;      /* n - previous value */
    }
    return val;
}
