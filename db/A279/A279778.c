/*
 * A279778  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 6/5.
 * https://oeis.org/A279778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 46, i32 n <= 96, i64 n <= 197,
 *            i128 n <= 392, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279778 – coefficients of (1-x)(1-x^5)/(1+x+x^2+x^3+2x^4)
   Recurrence:
   a(0)=1, a(1)=-2, a(2)=1, a(3)=0
   For n≥4
      a(n) = -(a(n-1)+a(n-2)+a(n-3)+2*a(n-4))
   with an extra +1 added to the RHS for n=5 and n=6
   (i.e. a(n)+a(n-1)+a(n-2)+a(n-3)+2*a(n-4)=
        1 if n=0, –1 if n=1 or 5, –1 if n=5, 1 if n=6, 0 otherwise) */

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* Circular buffer for the last four terms:
       prev4 = a(n-4), prev3 = a(n-3), prev2 = a(n-2), prev1 = a(n-1)
       curr   = a(n)                                 */
    seq_t prev4 = (seq_t)1;      /* a0 */
    seq_t prev3 = (seq_t)-2;     /* a1 */
    seq_t prev2 = (seq_t)1;      /* a2 */
    seq_t prev1 = (seq_t)0;      /* a3 */
    seq_t curr  = 0;             /* will hold a(n) */

    /* Compute terms from 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cn = 0;
        if (i == 5) cn = (seq_t)-1;
        else if (i == 6) cn = (seq_t)1;

        curr = cn - (prev1 + prev2 + prev3 + (seq_t)2 * prev4);

        /* Shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
