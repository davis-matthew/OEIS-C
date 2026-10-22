/*
 * A364084  Expansion of Sum_{k>0} k * x^(3*k) / (1 - x^(4*k-1)).
 * https://oeis.org/A364084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*Sum_{d|4*n-3, d==3 mod 4}(d+1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all divisors of 4*n-3. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = 4 * n - 3;          /* m = 4n-3 */
    seq_t sum = 0;

    /* Enumerate divisors d of m.  Using d <= m/d avoids overflow for large m. */
    for (idx_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            /* d is a divisor */
            if (d % 4 == 3)
                sum += ((seq_t)d + 1) / 4;

            /* m/d is the paired divisor */
            idx_t d2 = m / d;
            if (d2 != d && d2 % 4 == 3)
                sum += ((seq_t)d2 + 1) / 4;
        }
    }

    return sum;
}
