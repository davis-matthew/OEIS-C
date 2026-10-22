/*
 * A362343  Sequence that alternately doubles and squares the previous number;
 *          a(0) = 1.
 * https://oeis.org/A362343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = a(n-1)*2 if n is odd, otherwise a(n-1)^2 for n
 *            >= 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;            /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1)
            result *= 2;        /* odd step: double */
        else
            result = result * result;  /* even step: square */
    }
    return result;               /* a(n) */
}
