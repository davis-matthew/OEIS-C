/*
 * A348953  a(n) = -Sum_{d|n, d < sqrt(n)} (-1)^(d + n/d) * d.
 * https://oeis.org/A348953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d < sqrt(n)} (-1)^(d + n/d) * d
 * Range:     i8 n <= 989, i16 n <= 9493, i32 n <= 9493, i64 n <= 9493,
 *            i128 n <= 9493, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* non‑positive indices are unused */

    seq_t sum = 0;

    /* iterate over divisors d with d <= n/d; this safely avoids overflow */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            if (d == e)          /* skip the sqrt divisor for perfect squares */
                continue;

            /* (-1)^(d+e) is +1 when d+e is even, -1 when odd */
            if (((d + e) & 1U) == 0)
                sum += (seq_t)d;
            else
                sum -= (seq_t)d;
        }
    }

    return -sum;
}
