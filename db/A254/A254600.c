/*
 * A254600  Numbers of words on alphabet {0,1,...,10} with no subwords ii, for
 *          i from {0,1}.
 * https://oeis.org/A254600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 9*a(n-2); a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
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
        return (seq_t)11;

    seq_t a_prevprev = (seq_t)1;      /* a(n-2) */
    seq_t a_prev     = (seq_t)11;     /* a(n-1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = a_prev * (seq_t)10 + a_prevprev * (seq_t)9;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_prev;
}
