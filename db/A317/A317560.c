/*
 * A317560  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 11*a(n-2) - 11*a(n-3) - 72*a(n-4) - 54*a(n-5)
 *            for n > 6, with initial values: a(1)=4, a(2)=32, a(3)=228,
 *            a(4)=1619, a(5)=11510, a(6)=81814
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle offset */
    if (n == 0)
        return (seq_t)0;

    /* initial terms */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)32;
    if (n == 3) return (seq_t)228;
    if (n == 4) return (seq_t)1619;
    if (n == 5) return (seq_t)11510;
    if (n == 6) return (seq_t)81814;

    /* recurrence for n > 6, keeping only the last five values */
    seq_t a5 = (seq_t)11510;   /* a(n-1)   */
    seq_t a4 = (seq_t)1619;    /* a(n-2)   */
    seq_t a3 = (seq_t)228;     /* a(n-3)   */
    seq_t a2 = (seq_t)32;      /* a(n-4)   */
    seq_t a1 = (seq_t)4;       /* a(n-5)   */
    seq_t an = (seq_t)81814;   /* a(n)     */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = an * 6 + a5 * 11 - a4 * 11 - a3 * 72 - a2 * 54;
        /* shift window */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
        an = next;
    }
    return an;
}
