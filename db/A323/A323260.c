/*
 * A323260  Number of horizontally convex polyoctagons containing n regular
 *          polygons (squares or octagons).
 * https://oeis.org/A323260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4) - a(n-5)
 *            Initial values: a(0)=0, a(1)=2, a(2)=6, a(3)=24, a(4)=96,
 *            a(5)=390
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)24;
    if (n == 4) return (seq_t)96;
    if (n == 5) return (seq_t)390;

    /* Iterative recurrence, keeping only the last five values */
    seq_t a1 = (seq_t)2;   /* a(n-1) */
    seq_t a2 = (seq_t)6;   /* a(n-2) */
    seq_t a3 = (seq_t)24;  /* a(n-3) */
    seq_t a4 = (seq_t)96;  /* a(n-4) */
    seq_t a5 = (seq_t)390; /* a(n-5) */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = 5 * a5 - 3 * a4 - 5 * a3 + 7 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
