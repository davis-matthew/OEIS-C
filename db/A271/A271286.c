/*
 * A271286  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 337", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, a(2)=40, a(3)=200;
 *            a(n)=7*a(n-1)-14*a(n-2)+8*a(n-3) for n>3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)40;
    if (n == 3) return (seq_t)200;

    seq_t a1 = (seq_t)4;     /* a(n-3)  */
    seq_t a2 = (seq_t)40;    /* a(n-2)  */
    seq_t a3 = (seq_t)200;   /* a(n-1)  */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)7 * a3 - (seq_t)14 * a2 + (seq_t)8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
