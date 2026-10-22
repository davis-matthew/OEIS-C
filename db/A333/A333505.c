/*
 * A333505  a(n) = Sum_{k=1..n} (-1)^(k+1) * k * ceiling(n/k).
 * https://oeis.org/A333505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * k * ceil(n/k)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* ceil(n/k) = (n + k - 1) / k */
        seq_t kk = (seq_t)k;
        seq_t ceil_val = ((seq_t)n + kk - 1) / kk;
        seq_t term = ceil_val * kk;     /* k * ceil(n/k) */
        if ((k & 1) == 0)               /* (-1)^(k+1) is negative for even k */
            term = -term;
        sum += term;
    }
    return sum;
}
