/*
 * A337943  Total number of divisors, d, of the sum of the squared parts in
 *          each partition of n into two parts, where d < n.
 * https://oeis.org/A337943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..⌊n/2⌋} #{d ≤ n : d | (i² + (n‑i)²)}
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n >> 1;                    /* floor(n/2) */
    seq_t total = 0;

    for (idx_t i = 1; i <= half; ++i)
    {
        unsigned long long ii = (unsigned long long)i;
        unsigned long long nn = (unsigned long long)(n - i);
        unsigned long long s  = ii * ii + nn * nn;   /* i² + (n‑i)² */

        seq_t cnt = 0;
        unsigned long long lim = (unsigned long long)n;

        /* Enumerate divisors up to min(n, sqrt(s)) */
        for (unsigned long long d = 1; d <= lim && d * d <= s; ++d)
        {
            if (s % d == 0)
            {
                ++cnt;                 /* divisor d itself */

                unsigned long long other = s / d;
                if (other != d && other <= lim)
                    ++cnt;             /* second factor, if distinct and ≤ n */
            }
        }

        total += cnt;
    }

    return total;
}
