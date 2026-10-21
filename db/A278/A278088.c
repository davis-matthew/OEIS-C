/*
 * A278088  Number of n X 2 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2, a(3)=3, a(4)=10, a(5)=27
   For n>5: a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3)
   (derived from the OEIS entry for A278088) */

seq_t a(idx_t n)
{
    /* Return the small fixed values directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)27;

    /* Maintain the last three terms:
       a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)3;  /* a(3)  */
    seq_t a2 = (seq_t)10; /* a(4)  */
    seq_t a3 = (seq_t)27; /* a(5)  */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t cur = a3 + 4 * a2 + 3 * a1; /* a(i) */
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3; /* a(n) */
}
