/*
 * A249349  (A001147(n+1)-1)/2, equals the index of A249348(n) within the
 *          triangular numbers A000217.
 * https://oeis.org/A249349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+3)*a(n-1) + (1-4*n)*a(n-2) + (2*n-3)*a(n-3) a(0)=0,
 *            a(1)=1, a(2)=7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)7;

    seq_t a0 = (seq_t)0;           /* a(n-3)  */
    seq_t a1 = (seq_t)1;           /* a(n-2)  */
    seq_t a2 = (seq_t)7;           /* a(n-1)  */

    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        seq_t ti = (seq_t)i;

        /* compute (2*i+3)*a2 */
        seq_t term1 = (ti * (seq_t)2 + (seq_t)3) * a2;

        /* compute (2*i-3)*a0 */
        seq_t term2 = (ti * (seq_t)2 - (seq_t)3) * a0;

        /* compute (4*i-1)*a1  and subtract it */
        seq_t term3 = (ti * (seq_t)4 - (seq_t)1) * a1;

        seq_t a_next = term1 + term2;
        a_next -= term3;                /* a_next = term1 + term2 - term3 */

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;   /* a(n) */
}
