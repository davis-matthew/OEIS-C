/*
 * A294279  Sum of the tenth powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i^10 + (n-i)^10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 70, u128 n <= 3959,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t power10(idx_t base)
{
    seq_t res = 1;
    for (int k = 0; k < 10; ++k)
        res *= (seq_t)base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n >> 1;  /* floor(n/2) */
    for (idx_t i = 1; i <= limit; ++i)
    {
        seq_t pi = power10(i);
        seq_t pj = power10(n - i);
        sum += pi;
        sum += pj;
    }
    return sum;
}
