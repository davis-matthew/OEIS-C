/*
 * A284326  Sum of the divisors of n that are not divisible by 6.
 * https://oeis.org/A284326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, 6∤d} d
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of all divisors of n that are not multiples of 6. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    /* Enumerate every divisor d of n.  n is small enough for
       an O(n) loop (up to about 10 000 in the tests).  */
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (d % 6 != 0)
                total += (seq_t)d;
        }
    }
    return total;
}
