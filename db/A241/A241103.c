/*
 * A241103  Number of n X 3 0..2 arrays with no element equal to one or three
 *          horizontal or vertical neighbors, with new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A241103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) + 16*a(n-3) Initial values: a(1) = 2,
 *            a(2) = 11, a(3) = 58 OEIS sequence A241103: Number of n×3 0..2
 *            arrays with no element equal to horizontal or vertical
 *            neighbours
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for OEIS offset 1, but harmless */
    }
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)58;

    /* Initialize three successive terms */
    seq_t a1 = (seq_t)2,  /* a(n-3)  */
          a2 = (seq_t)11, /* a(n-2)  */
          a3 = (seq_t)58; /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Recurrence: a(i) = 3*a(i-1) + 8*a(i-2) + 16*a(i-3) */
        next = (seq_t)3 * a3 + (seq_t)8 * a2 + (seq_t)16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
