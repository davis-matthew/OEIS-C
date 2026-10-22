/*
 * A302064  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2 or
 *          4 horizontally or antidiagonally adjacent elements, with upper
 *          left element zero.
 * https://oeis.org/A302064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=25, a(3)=148; a(n)=8*a(n-1)-12*a(n-2) for n>3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)148;

    seq_t prevprev = (seq_t)25;   /* a(2) */
    seq_t prev     = (seq_t)148;  /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)8 * prev - (seq_t)12 * prevprev;
        prevprev = prev;
        prev = next;
    }
    return prev;
}
