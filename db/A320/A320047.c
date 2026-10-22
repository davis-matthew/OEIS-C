/*
 * A320047  Consider coefficients U(m,l,k) defined by the identity
 *          Sum_{k=1..l} Sum_{j=0..m} A302971(m,j)/A304042(m,j) * k^j *
 *          (T-k)^j = Sum_{k=0..m} (-1)^(m-k) * U(m,l,k) * T^k that holds for
 *          all positive integers l,m,T. This sequence gives 2-column table
 *          read by rows, where n-th row lists coefficients U(1,...
 * https://oeis.org/A320047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A320047
 * a(n) = { 3*n/4*(n+2) if n even
 *          (n^3+6*n^2+9*n+4)/4 if n odd }
 * (see recurrence a(n)=4*a(n-2)-6*a(n-4)+4*a(n-6)-a(n-8))
 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0)          /* even n */
    {
        seq_t k = nn >> 1;     /* n/2 */
        return (seq_t)3 * k * (k + (seq_t)1);  /* 3*k*(k+1) = 3*n*(n+2)/4 */
    }
    else                       /* odd n */
    {
        seq_t n2 = nn * nn;   /* n^2 */
        seq_t n3 = n2 * nn;   /* n^3 */
        seq_t term = n3 + (seq_t)6 * n2 + (seq_t)9 * nn + (seq_t)4;
        return term / (seq_t)4;
    }
}
