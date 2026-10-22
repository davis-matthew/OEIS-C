/*
 * A368853  a(n) = n! * (n+1)! * Sum_{k=0..n} (-1)^k/(k! * (k+1)!).
 * https://oeis.org/A368853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*a(n-1) + (-1)^n for n ≥ 1, with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_n = 1;       /* a(0) */
    if (n == 0) return a_n;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = (seq_t)i * ((seq_t)i + 1);  /* n*(n+1) */
        seq_t temp = mult * a_n;                 /* n*(n+1)*a(n-1) */
        if ((i & 1) == 0)   /* n even: add 1 */
            a_n = temp + 1;
        else                /* n odd: subtract 1 */
            a_n = temp - 1;
    }
    return a_n;
}
