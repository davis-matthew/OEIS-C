/*
 * A366506  a(n) = Lucas(n) + 3.
 * https://oeis.org/A366506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) + 3 Recurrence: a(1)=4, a(2)=6, a(3)=7,
 *            a(n)=2*a(n-1)-a(n-3) for n≥4
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)6;
    if (n == (idx_t)3) return (seq_t)7;

    /* keep the last three values only */
    seq_t a_n_minus3 = (seq_t)4; /* a(n-3) */
    seq_t a_n_minus2 = (seq_t)6; /* a(n-2) */
    seq_t a_n_minus1 = (seq_t)7; /* a(n-1) */
    seq_t a_n;                   /* a(n) */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a_n = a_n_minus1 * (seq_t)2 - a_n_minus3;
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = a_n;
    }
    return a_n;
}
