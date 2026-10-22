/*
 * A332190  a(n) = 10^(2n+1) - 1 - 9*10^n.
 * https://oeis.org/A332190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2n+1) - 1 - 9*10^n Recurrence used for efficient
 *            computation: a(0) = 0, a(1) = 909, a(2) = 99099, a(n) =
 *            111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) for n > 2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)909;
    if (n == 2) return (seq_t)99099;

    seq_t a_nm3 = 0;            /* a(n-3) */
    seq_t a_nm2 = 909;          /* a(n-2) */
    seq_t a_nm1 = 99099;        /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t t1 = a_nm1 * (seq_t)111;
        seq_t t2 = a_nm2 * (seq_t)1110;
        seq_t t3 = a_nm3 * (seq_t)1000;
        seq_t cur = t1 + t3 - t2;

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
