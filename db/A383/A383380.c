/*
 * A383380  Expansion of e.g.f. exp(-2*x) / (1-x)^4.
 * https://oeis.org/A383380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=2; a(n) = (n+1)*a(n-1) + 2*(n-1)*a(n-2) for n ≥ 2
 *            (this is the recurrence given in the OEIS entry A383380)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)2; /* a(n-1) */
    seq_t a_n;             /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = (seq_t)(i + 1) * a_nm1          /* (i+1)*a(i-1) */
            + (seq_t)2 * (seq_t)(i - 1) * a_nm2; /* 2*(i-1)*a(i-2) */

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
