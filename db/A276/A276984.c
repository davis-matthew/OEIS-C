/*
 * A276984  Sum of squares of numbers less than n that do not divide n.
 * https://oeis.org/A276984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{1 ≤ i < n, i ∤ n} i²
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 2344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 0, undefined for n = 0 */
        return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i)
    {
        if (n % i != 0)
        {
            seq_t v = (seq_t)i;
            sum += v * v;
        }
    }
    return sum;
}
