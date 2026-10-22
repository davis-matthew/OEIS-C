/*
 * A317512  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5, 6 or 7 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - a(n-2) + 6*a(n-3), a(1)=4, a(2)=32, a(3)=255
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined by definition, but safe */

    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)32;
    if (n == 3)
        return (seq_t)255;

    seq_t a1 = (seq_t)4,   /* a(n-3) for i=4 */
          a2 = (seq_t)32,  /* a(n-2) */
          a3 = (seq_t)255; /* a(n-1) */
    seq_t an;              /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)8 * a3 - a2 + (seq_t)6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
