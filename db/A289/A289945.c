/*
 * A289945  a(n) = Sum_{k=1..n} k!^4.
 * https://oeis.org/A289945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k!^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t termPow4 = 1;   /* (k-1)!^4 at the start of each loop */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* i^4 in a small integer type; safe for i <= 10000 */
        seq_t ipow = (seq_t)i;
        ipow *= ipow;          /* i^2 */
        ipow *= i;             /* i^3 */
        ipow *= i;             /* i^4 */

        termPow4 *= ipow;      /* (k-1)!^4 * i^4 = k!^4 */
        sum += termPow4;       /* accumulate the sum */
    }
    return sum;
}
