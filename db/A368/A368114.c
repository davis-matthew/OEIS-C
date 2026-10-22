/*
 * A368114  Trajectory of 11 under the '11x+1' map.
 * https://oeis.org/A368114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=11; for n>0, a(n)=f(a(n-1)) with f(x) = x/2 if x even x/3
 *            if x odd and divisible by 3 x/7 if x odd and divisible by 7
 *            11*x+1 otherwise
 * Range:     u8 n <= 2, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A368114. */
seq_t a(idx_t n)
{
    seq_t x = 11;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((x & 1) == 0)          /* even */
        {
            x = x / 2;
        }
        else if (x % 3 == 0)       /* odd, divisible by 3 */
        {
            x = x / 3;
        }
        else if (x % 7 == 0)       /* odd, divisible by 7 */
        {
            x = x / 7;
        }
        else                       /* odd, not divisible by 3 or 7 */
        {
            x = 11 * x + 1;
        }
    }
    return x;
}
