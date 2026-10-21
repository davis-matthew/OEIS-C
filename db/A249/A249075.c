/*
 * A249075  Sum of the numbers in row n of the array at A249074.
 * https://oeis.org/A249075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*(n+1)*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a_prevprev = (seq_t)1;   /* a(0) */
    seq_t a_prev     = (seq_t)5;   /* a(1) */
    seq_t a_curr     = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*(i+1)*a(i-2) */
        seq_t term = (seq_t)2 * (seq_t)(i + 1) * a_prevprev;
        a_curr = a_prev + term;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_curr;
}
