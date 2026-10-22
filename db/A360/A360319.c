/*
 * A360319  a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n-1,n-k) * binomial(2*k,k).
 * https://oeis.org/A360319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * binomial(n-1,n-k) *
 *            binomial(2*k,k) Recurrence used: n*a(n) = 2*(6*n-5)*a(n-1) -
 *            32*(n-2)*a(n-2), with a(0) = 1, a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a_nm2 = (seq_t)1;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)2;          /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t six_i = (seq_t)6 * i_seq;
        seq_t term1_factor = six_i - (seq_t)5;          /* 6*i-5 */
        seq_t term1 = (seq_t)2 * term1_factor * a_nm1;   /* 2*(6*i-5)*a(i-1) */
        seq_t term2 = (seq_t)32 * (i_seq - (seq_t)2) * a_nm2; /* 32*(i-2)*a(i-2) */
        seq_t rhs = term1 - term2;                       /* n*a(i) */
        seq_t a_i = rhs / i_seq;                         /* a(i) = rhs / i */
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
