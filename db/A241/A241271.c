/*
 * A241271  a(n) = 6*a(n-1) + 3*(2^(n-2)-1) for n > 2, a(0)=a(1)=a(2)=0.
 * https://oeis.org/A241271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*(2^(n-2)-1) for n>2 initial values: a(0) =
 *            a(1) = a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u)
        return (seq_t)0;

    seq_t a_prev = (seq_t)0;     /* a(2) */
    seq_t pow2   = (seq_t)2;     /* 2^(3-2) */

    for (idx_t i = 3u; i <= n; ++i) {
        seq_t term = (seq_t)6 * a_prev + (seq_t)3 * (pow2 - (seq_t)1);
        a_prev = term;
        pow2  *= (seq_t)2;
    }

    return a_prev;
}
