/*
 * A294304  Sum of the ninth powers of the parts of the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} i^9 + (n-i)^9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 106, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow9(seq_t x)
{
    /* Compute x^9 by repeated multiplication. */
    return x * x * x * x * x * x * x * x * x;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;           /* floor((n-1)/2) */
    for (idx_t i = 1; i <= limit; ++i)
    {
        seq_t p1 = pow9((seq_t)i);                   /* i^9   */
        seq_t p2 = pow9((seq_t)(n - i));             /* (n-i)^9 */
        sum += p1 + p2;
    }
    return sum;
}
