/*
 * A251367  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 1, 2, 3, 4, 5, 6, or 7.
 * https://oeis.org/A251367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) + 49*a(n-2) - 34*a(n-3); Initial values:
 *            a(1)=695, a(2)=17969, a(3)=464393
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)695;
    if (n == 2) return (seq_t)17969;
    if (n == 3) return (seq_t)464393;

    seq_t prev3 = (seq_t)695;   /* a(i-3)   */
    seq_t prev2 = (seq_t)17969; /* a(i-2)   */
    seq_t prev1 = (seq_t)464393;/* a(i-1)   */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        curr = 24 * prev1 + 49 * prev2 - 34 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
