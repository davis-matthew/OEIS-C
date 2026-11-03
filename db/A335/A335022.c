/*
 * A335022  a(n) = Sum_{d|n, 1 < d < n} (-1)^(d + 1) * d.
 * https://oeis.org/A335022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, 1<d<n} (-1)^(d+1) * d
 * Range:     i8 n <= 95, i16 n <= 13439, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9993 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 2; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t sd = (seq_t)d;
            sum += ((d & 1) ? 1 : -1) * sd;
            idx_t other = n / d;
            if (other != d)
            {
                seq_t so = (seq_t)other;
                sum += ((other & 1) ? 1 : -1) * so;
            }
        }
    }
    return sum;
}
