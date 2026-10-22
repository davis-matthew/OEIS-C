/*
 * A360066  Number of 3-dimensional tilings of a 2 X 2 X n box using 1 X 1 X 1
 *          cubes, 2 X 1 X 1 dominos and trominos (L-shaped connection of 3
 *          cubes).
 * https://oeis.org/A360066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 11*a(n-1)+4*b(n-1)+2*c(n-1)+d(n-1)+e(n-1)
          +29*a(n-2)+6*b(n-2)+c(n-2)+2*d(n-2)
   b(n) = 32*a(n-1)+9*b(n-1)+4*c(n-1)+2*d(n-1)+e(n-1)
   c(n) = 52*a(n-1)+14*b(n-1)+5*c(n-1)+4*d(n-1)+2*e(n-1)
   d(n) = 14*a(n-1)+3*b(n-1)+d(n-1)
   e(n) = 48*a(n-1)+11*b(n-1)+2*c(n-1)+2*d(n-1)
   Initial: a(0)=1, b(0)=c(0)=d(0)=e(0)=0
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) */

    /* State variables for (n-1) and (n-2) */
    seq_t pa = (seq_t)1;     /* a(n-1) at start is a(0) */
    seq_t pb = 0;
    seq_t pc = 0;
    seq_t pd = 0;
    seq_t pe = 0;

    seq_t pa2 = 0; /* a(n-2), all zero for n-2 < 0 */
    seq_t pb2 = 0;
    seq_t pc2 = 0;
    seq_t pd2 = 0;
    seq_t pe2 = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute state for current n (i) */
        seq_t ca = 11*pa + 4*pb + 2*pc + pd + pe          /* terms with (n-1) */
                 + 29*pa2 + 6*pb2 + pc2 + 2*pd2;          /* terms with (n-2) */

        seq_t cb = 32*pa + 9*pb + 4*pc + 2*pd + pe;
        seq_t cc = 52*pa + 14*pb + 5*pc + 4*pd + 2*pe;
        seq_t cd = 14*pa + 3*pb + pd;
        seq_t ce = 48*pa + 11*pb + 2*pc + 2*pd;

        /* shift states for next iteration */
        pa2 = pa;  pb2 = pb;  pc2 = pc;  pd2 = pd;  pe2 = pe;
        pa  = ca;  pb  = cb;  pc  = cc;  pd  = cd;  pe  = ce;
    }

    return pa; /* a(n) */
}
