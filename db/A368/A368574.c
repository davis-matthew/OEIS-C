/*
 * A368574  a(n) = n! * Sum_{k=0..n} binomial(k+2,3) / k!.
 * https://oeis.org/A368574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n) = n*a(n-1) + binomial(n+2,3). */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t binom = ((seq_t)(i + 2) * (seq_t)(i + 1) * (seq_t)i) / (seq_t)6;
        res = (seq_t)i * res + binom;
    }
    return res;
}
