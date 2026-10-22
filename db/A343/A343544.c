/*
 * A343544  a(n) = n * Sum_{d|n} binomial(d+2,3)/d.
 * https://oeis.org/A343544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} (d+2)(d+1)/6 (derived from a(n) = n *
 *            Sum_{d|n} binomial(d+2,3)/d)
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 1802, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t d;

    for (d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t d1 = (seq_t)d;
            sum += (d1 + 2) * (d1 + 1);

            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t d2s = (seq_t)d2;
                sum += (d2s + 2) * (d2s + 1);
            }
        }
    }

    return (seq_t)n * sum / 6;
}
