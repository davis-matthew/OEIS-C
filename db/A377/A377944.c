/*
 * A377944  Cogrowth sequence of the 16-element dicyclic group Q16 = <S,T |
 *          S^8, T^4, (ST)^4, TST^3S>.
 * https://oeis.org/A377944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=12, *
 *            a(n)=2*a(n-1)+4*a(n-2)+16*a(n-3) for n>=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)12;

    /* initialize a(1), a(2), a(3) */
    seq_t a_n_minus_3 = (seq_t)0;   /* a(1)   */
    seq_t a_n_minus_2 = (seq_t)1;   /* a(2)   */
    seq_t a_n_minus_1 = (seq_t)12;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t a_n = (seq_t)2 * a_n_minus_1
                  + (seq_t)4 * a_n_minus_2
                  + (seq_t)16 * a_n_minus_3;

        /* shift window for next step */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n_minus_1;
}
