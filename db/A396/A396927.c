/*
 * A396927  Breadth-first search (BFS) layer sizes of the Coxeter group of the
 *          5-dimensional right-corner orthoscheme with five pairwise distinct
 *          leg lengths.
 * https://oeis.org/A396927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) for n ≥ 4, with a(0)=1,
 *            a(1)=6, a(2)=20, a(3)=54
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)54;

    /* a_m3 = a(n-3), a_m2 = a(n-2), a_m1 = a(n-1) */
    seq_t a_m3 = (seq_t)6;   /* a(1) */
    seq_t a_m2 = (seq_t)20;  /* a(2) */
    seq_t a_m1 = (seq_t)54;  /* a(3) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = 3 * a_m1 - a_m2 - a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = an;
    }

    return a_m1;
}
