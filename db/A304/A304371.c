/*
 * A304371  Number of function calls of the second kind required to compute
 *          ack(3,n), where ack denotes the Ackermann function.
 * https://oeis.org/A304371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=47, a(2)=257, a(3)=1187,
 *            a(n)=8*a(n-1)-21*a(n-2)+22*a(n-3)-8*a(n-4) for n≥4
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 5;
    if (n == 1) return 47;
    if (n == 2) return 257;
    if (n == 3) return 1187;

    seq_t prev4 = 5,   /* a(n-4)   */
          prev3 = 47,  /* a(n-3)   */
          prev2 = 257, /* a(n-2)   */
          prev1 = 1187;/* a(n-1)   */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = 8 * prev1 - 21 * prev2 + 22 * prev3 - 8 * prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
