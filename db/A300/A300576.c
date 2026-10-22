/*
 * A300576  Number of nights required in the worst case to find the princess
 *          in a castle with n rooms arranged in a line (Castle and princess
 *          puzzle).
 * https://oeis.org/A300576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n)=2*n-4 for n>=3
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* outside the defined offset */
    if (n <= 2)
        return (seq_t)n;          /* a(1)=1, a(2)=2 */

    return (seq_t)2 * (seq_t)n - (seq_t)4;   /* a(n)=2*n-4 for n>=3 */
}
