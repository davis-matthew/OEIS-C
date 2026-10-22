/*
 * A367963  a(n) = Sum_{j=0..n} (2*n)! / (n!*(n - j)!).
 * https://oeis.org/A367963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (2*n)! / (n!*(n - j)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (2n)! / n! as product_{i=1}^{n} (n + i) */
    seq_t term = 1;
    for (idx_t i = 1; i <= n; ++i)
        term = term * (seq_t)(n + i);

    /* accumulate sum_{k=0}^{n} term / k! by successive division */
    seq_t sum = 0;
    seq_t cur = term;
    for (idx_t k = 0; k <= n; ++k)
    {
        sum += cur;
        if (k < n)
            cur = cur / (seq_t)(k + 1);
    }
    return sum;
}
