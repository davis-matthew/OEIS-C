/*
 * A363606  Expansion of Sum_{k>0} x^(2*k)/(1-x^k)^6.
 * https://oeis.org/A363606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(d+3,5)
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 217, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = Sum_{d|n} (d+3)(d+2)(d+1)d(d-1)/120 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* first divisor d = i */
            idx_t d = i;
            seq_t term = 1;
            for (idx_t j = 0; j < 5; ++j)
                term = term * ( (seq_t)(d + 3) - j) / (j + 1);
            sum += term;

            /* second divisor d = n / i, avoid double count */
            idx_t d2 = n / i;
            if (d2 != d)
            {
                term = 1;
                for (idx_t j = 0; j < 5; ++j)
                    term = term * ( (seq_t)(d2 + 3) - j) / (j + 1);
                sum += term;
            }
        }
    }
    return sum;
}
