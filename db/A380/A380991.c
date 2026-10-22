/*
 * A380991  a(n) = largest number of cells of a polyomino with at most n
 *          collinear cell centers.
 * https://oeis.org/A380991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, hard, more
 * Formula:   a(1) = 1; a(2) = 4; a(n) = 3 * a(n-1) + 3, for n >= 3
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n <= 1)       return (n == 1) ? (seq_t)1 : (seq_t)0; /* n==0 safe */
    if (n == 2)       return (seq_t)4;

    /* Compute iteratively for n >= 3 */
    seq_t val = (seq_t)4;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
        val = val * 3 + 3;  /* a(i) = 3*a(i-1) + 3 */

    return val;
}
