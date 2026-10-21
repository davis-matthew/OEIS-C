/*
 * A267850  Binary representation of the n-th iteration of the "Rule 229"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2), a(0)=1, a(1)=10, a(2)=11110,
 *            a(3)=1111110
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 11110;
    if (n == 3) return 1111110;

    /* Use the recurrence for n > 3:
       a(n) = 101*a(n-1) - 100*a(n-2)    */
    seq_t prev2 = 11110;      /* a(n-2) */
    seq_t prev1 = 1111110;    /* a(n-1) */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        curr = prev1 * 101 - prev2 * 100;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
