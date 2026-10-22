/*
 * A360263  Determinant of the pentadiagonal symmetric nXn Toeplitz Matrix
 *          with a=3, b=c=1.
 * https://oeis.org/A360263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360263
   Recurrence:  a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - 2*a(n-4) + a(n-5)
   Initial values:  a(0)=1,  a(1)=3,  a(2)=8,  a(3)=20,  a(4)=48
*/
seq_t a(idx_t n)
{
    /* handle the first five terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)48;

    /* last five computed values (a_{n-5} … a_{n-1}) */
    seq_t a0 = (seq_t)1;   /* a_{i-5} */
    seq_t a1 = (seq_t)3;   /* a_{i-4} */
    seq_t a2 = (seq_t)8;   /* a_{i-3} */
    seq_t a3 = (seq_t)20;  /* a_{i-2} */
    seq_t a4 = (seq_t)48;  /* a_{i-1} */

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a_i = 2*a4 + 2*a3 - 2*a2 - 2*a1 + a0
           Structure the calculation to avoid negative intermediates
           for unsigned seq_t types. */
        seq_t tmp = a4 + a3;
        tmp = tmp * (seq_t)2;     /* 2*(a4 + a3) */
        seq_t sub = a2 + a1;
        sub = sub * (seq_t)2;     /* 2*(a2 + a1) */
        seq_t a5 = tmp - sub + a0;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }

    return a4;   /* a(n) is stored in a4 after the loop */
}
