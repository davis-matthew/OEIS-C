/*
 * A258384  a(n) = n^(n-1) * (n+1)^n.
 * https://oeis.org/A258384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n-1) * (n+1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t term1 = _pow_seq((seq_t)n, n - 1);       /* n^(n-1)  */
    seq_t term2 = _pow_seq((seq_t)(n + 1), n);     /* (n+1)^n  */
    return term1 * term2;                         /* product  */
}
