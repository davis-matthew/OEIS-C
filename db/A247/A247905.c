/*
 * A247905  Start with a single hexagon; at n-th generation add a hexagon at
 *          each expandable vertex (this is the "vertex to side" version);
 *          a(n) is the sum of all label values at n-th generation. (See
 *          comment for construction rules.)
 * https://oeis.org/A247905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 245,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=19, a(3)=43,
   a(n)=2*a(n-1)+a(n-2)-4*a(n-3)+2*a(n-4). */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)43;

    /* a[0]..a[3] hold a(n-4), a(n-3), a(n-2), a(n-1).  */
    seq_t a[4];
    a[0] = (seq_t)1;   /* a(0)   */
    a[1] = (seq_t)7;   /* a(1)   */
    a[2] = (seq_t)19;  /* a(2)   */
    a[3] = (seq_t)43;  /* a(3)   */

    seq_t next;
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 2*a(i-1)+a(i-2)-4*a(i-3)+2*a(i-4)
           using only unsigned arithmetic.  The expression
           (2*a(i-1)+a(i-2)+2*a(i-4)) is guaranteed to be
           greater than or equal to (4*a(i-3)). */
        seq_t temp = (a[3] * 2) + a[2] + (a[0] * 2);
        seq_t mult4 = a[1] * 4;
        next = temp - mult4;

        /* Shift the window for the next iteration. */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = next;
    }

    return a[3];
}
