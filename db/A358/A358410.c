/*
 * A358410  a(n) = Sum_{d|n} (d + n/d - 2)!/(d - 1)!.
 * https://oeis.org/A358410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d + n/d - 2)!/(d - 1)!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; optional guard */
    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) /* iterate up to sqrt(n) safely */
    {
        if (n % d == 0)
        {
            idx_t q1 = n / d;        /* first divisor pair (d, q1) */

            /* compute product_{i=0}^{q1-2} (d + i) */
            seq_t term1 = 1;
            for (idx_t i = 0; i < q1 - 1; ++i)
                term1 *= (seq_t)(d + i);
            sum += term1;

            /* if d != q1, include the symmetric divisor pair (q1, d) */
            if (q1 != d)
            {
                idx_t d2 = q1;
                idx_t q2 = d;

                seq_t term2 = 1;
                for (idx_t i = 0; i < q2 - 1; ++i)
                    term2 *= (seq_t)(d2 + i);
                sum += term2;
            }
        }
    }

    return sum;
}
