/*
 * A398811  a(n) is the number of tilings of a 10 X 5n rectangle into
 *          Y-pentominoes (no reflections, only rotations).
 * https://oeis.org/A398811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)+5*a(n-2) for n > 3 Initial values: a(0)=1,
 *            a(1)=1, a(2)=8, a(3)=44 Implements OEIS sequence A398811
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the known small cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)44;

    /* iterate from n = 4 up to the requested n */
    seq_t prev2 = (seq_t)8;   /* a(n-2) */
    seq_t prev1 = (seq_t)44;  /* a(n-1) */
    seq_t curr;               /* a(n) */
    idx_t i;

    for (i = 4; i <= n; ++i) {
        curr = 5 * prev1 + 5 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
