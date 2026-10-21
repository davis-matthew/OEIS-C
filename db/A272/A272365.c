/*
 * A272365  a(n) = 9a(n-1) - 9a(n-2) + a(n-3).
 * https://oeis.org/A272365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3) with initial values a(1)=10,
 *            a(2)=34, a(3)=250 (defined for n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)34;
    if (n == 3) return (seq_t)250;
    /* For n < 1 the sequence is undefined; return 0 as a safe default. */
    if (n == 0) return (seq_t)0;

    /* Recursively compute the terms up to the requested index.
       We maintain only the last three values to keep memory usage minimal. */
    seq_t a_nm3 = (seq_t)10;   /* a(n-3) for n=4 */
    seq_t a_nm2 = (seq_t)34;   /* a(n-2) for n=4 */
    seq_t a_nm1 = (seq_t)250;  /* a(n-1) for n=4 */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t cur = a_nm1 * (seq_t)9 - a_nm2 * (seq_t)9 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
