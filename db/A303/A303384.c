/*
 * A303384  Total area of all rectangles with dimensions s and t where s | t,
 *          n = s + t and s <= t.
 * https://oeis.org/A303384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n} d * (n - d)
 * Range:     u8 n <= 19, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            sum += (seq_t)d1 * ((seq_t)n - d1);
            idx_t d2 = n / i;
            if (d2 != d1)
                sum += (seq_t)d2 * ((seq_t)n - d2);
        }
    }
    return sum;
}
