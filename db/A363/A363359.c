/*
 * A363359  Sum of divisors of 4*n-1 of form 4*k+3.
 * https://oeis.org/A363359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of (4*n - 1) that are congruent to 3
 *            modulo 4
 * Range:     u8 n <= 53, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 4 * n - 1;       /* integer m = 4*n - 1 */
    seq_t sum = 0;             /* accumulator for the sum */

    /* Enumerate all divisors i of m, i <= sqrt(m). Use m / i to avoid overflow. */
    for (idx_t i = 1; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            idx_t j = m / i;  /* corresponding divisor */

            if (i % 4 == 3)
                sum += (seq_t)i;         /* add i if it satisfies the condition */

            if (j != i && j % 4 == 3)
                sum += (seq_t)j;         /* add j if it satisfies the condition and
                                            not counted already */
        }
    }

    return sum;
}
