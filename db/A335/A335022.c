/*
 * A335022  a(n) = Sum_{d|n, 1 < d < n} (-1)^(d + 1) * d.
 * https://oeis.org/A335022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, 1<d<n} (-1)^(d+1) * d
 * Range:     i8 n <= 95, i16 n <= 9993, i32 n <= 9993, i64 n <= 9993,
 *            i128 n <= 9993, big any n
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
