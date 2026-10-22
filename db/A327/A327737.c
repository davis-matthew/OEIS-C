/*
 * A327737  a(n) is the sum of the lengths of the base-b expansions of n for
 *          all b with 1 <= b <= n.
 * https://oeis.org/A327737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*(n+1)/2 (integers per OEIS definition)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sum_logs = 0;
    for (idx_t b = 2; b <= n; ++b)
    {
        seq_t sqb = (seq_t)b;
        seq_t pow = 1;
        seq_t exp = 0;
        /* count how many times we can multiply by b while staying <= n */
        while (pow <= sn / sqb)
        {
            pow *= sqb;
            ++exp;
        }
        sum_logs += exp; /* floor(log_b(n)) */
    }
    return (sn * 2 - 1) + sum_logs;   /* 2n-1 + sum_{b=2}^{n} floor(log_b(n)) */
}
