/*
 * A384730  Expansion of (1+x-2*x^2) / (1-x-6*x^2+2*x^3).
 * https://oeis.org/A384730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=6;
 * a(n) = a(n-1) + 6*a(n-2) - 2*a(n-3)  (n ≥ 3) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a_nm3 = (seq_t)1;      /* a(n-3) */
    seq_t a_nm2 = (seq_t)2;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)6;      /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = a_nm1 + (seq_t)6 * a_nm2 - (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
