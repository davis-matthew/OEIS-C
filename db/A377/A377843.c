/*
 * A377843  Cogrowth sequence of the 16-element group C4 X C2 X C2 = <S,T,U |
 *          S^4, T^2, U^2, [S,T], [S,U], [T,U]>.
 * https://oeis.org/A377843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A377843 : Cogrowth of C4 × C2 × C2
    Recurrence (from the rational generating function):
        a(0)=1, a(1)=2, a(2)=9, a(3)=62, a(4)=689,
        a(n)=15*a(n-1)-78*a(n-2)+210*a(n-3)+79*a(n-4)-225*a(n-5)  for n≥5
    (OEIS offset 0)
 */
seq_t a(idx_t n)
{
    /* handle the first five values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)62;
    if (n == 4) return (seq_t)689;

    /* last five previously computed values */
    seq_t a0 = (seq_t)1;   /* a(n-5) */
    seq_t a1 = (seq_t)2;   /* a(n-4) */
    seq_t a2 = (seq_t)9;   /* a(n-3) */
    seq_t a3 = (seq_t)62;  /* a(n-2) */
    seq_t a4 = (seq_t)689; /* a(n-1) */

    seq_t ai;
    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)15 * a4            /* 15*a(n-1) */
           - (seq_t)78 * a3            /*  -78*a(n-2) */
           + (seq_t)210 * a2           /* +210*a(n-3) */
           + (seq_t)79 * a1            /* +79*a(n-4) */
           - (seq_t)225 * a0;          /* -225*a(n-5) */
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;   /* which is a(n) for the last iteration */
}
