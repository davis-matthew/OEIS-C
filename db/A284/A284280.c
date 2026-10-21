/*
 * A284280  a(n) = Sum_{d|n, d == 2 (mod 5)} d.
 * https://oeis.org/A284280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{d|n, d == 2 (mod 5)} d
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */
    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d)  /* iterate through all divisors */
    {
        if (n % d == 0)
        {
            if (d % 5 == 2)  sum += (seq_t)d;

            idx_t other = n / d;
            if (other != d && other % 5 == 2)
                sum += (seq_t)other;
        }
    }
    return sum;
}
