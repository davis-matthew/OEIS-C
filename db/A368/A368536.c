/*
 * A368536  a(n) = Sum_{k=1..n} binomial(k+1,2) * n^k.
 * https://oeis.org/A368536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{k=1..n} \binom{k+1}{2} * n^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t sum = 0;
    seq_t pow = (seq_t)n;          /* pow = n^1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t bin = (seq_t)k * (k + 1) / 2;   /* binomial(k+1,2) */
        sum += bin * pow;                    /* add term */
        if (k != n)                          /* compute next power */
            pow *= (seq_t)n;
    }
    return sum;
}
