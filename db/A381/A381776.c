/*
 * A381776  Empty polygon numbers: a(n) is the smallest number of points in
 *          the plane (with no three of them collinear) such that an empty
 *          convex n-gon cannot be avoided.
 * https://oeis.org/A381776
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, bref, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empty polygon numbers: a(3)=3, a(4)=5, a(5)=10, a(6)=30.
   For n>6 the sequence is undefined (empty convex n‑gons can be avoided
   by arbitrarily large point sets).  This function returns 0 for such
   n, which is a conventional placeholder for an undefined value. */
seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)30;
    return 0;  /* undefined for n > 6 */
}
