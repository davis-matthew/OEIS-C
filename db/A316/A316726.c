/*
 * A316726  The number of ways to tile (with squares and rectangles) a 2 X
 *          (n+2) strip with the upper left and upper right squares removed.
 * https://oeis.org/A316726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - a(n-4) for n>=4, with a(0)=2,
 *            a(1)=4, a(2)=15, a(3)=46
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)46;

    /* Initialise a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a0 = (seq_t)2;   /* a(n-4) */
    seq_t a1 = (seq_t)4;   /* a(n-3) */
    seq_t a2 = (seq_t)15;  /* a(n-2) */
    seq_t a3 = (seq_t)46;  /* a(n-1) */
    seq_t an = (seq_t)0;

    /* Compute iteratively up to n. */
    for (idx_t i = 4; i <= n; ++i)
    {
        an = 2 * a3 + 4 * a2 - a0;   /* a(i) = 2*a(i-1)+4*a(i-2)-a(i-4) */
        a0 = a1;  /* shift window: a(n-3) becomes a(n-4) */
        a1 = a2;  /* a(n-2) becomes a(n-3) */
        a2 = a3;  /* a(n-1) becomes a(n-2) */
        a3 = an;  /* a(i) becomes a(n-1) for next iteration */
    }
    return an;
}
