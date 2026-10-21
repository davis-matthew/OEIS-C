/*
 * A279927  Expansion of e.g.f. arctan(x)*exp(x).
 * https://oeis.org/A279927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋}
 *            binomial(n+1,2k+1)*(-1)^k*((n-2k)/(n+1))*(2k)!
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n+4) = 2*a(n+3) - (n²+5n+7)*a(n+2) + 2*(n+2)²*a(n+1) - (n+1)(n+2)*a(n) */
/* with initial values a(0)=0, a(1)=1, a(2)=2, a(3)=1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)1;   /* a(3) */
    seq_t an;

    for (idx_t i = 0; i <= n - 4; ++i) {
        seq_t term1 = (seq_t)2 * a3;                                    /* 2*a(n+3) */
        seq_t coeff2 = (seq_t)(i * i + (idx_t)5 * i + (idx_t)7);       /* n²+5n+7 */
        seq_t term2 = coeff2 * a2;                                      /* coeff2*a(n+2) */
        seq_t nn1 = (seq_t)(i + 1);                                      /* n+1 */
        seq_t nn2 = (seq_t)(i + 2);                                      /* n+2 */
        seq_t term3 = (seq_t)2 * nn2 * nn2 * a1;                         /* 2*(n+2)²*a(n+1) */
        seq_t term4 = nn1 * nn2 * a0;                                    /* (n+1)(n+2)*a(n) */
        an = term1 - term2 + term3 - term4;                             /* a(n+4) */

        /* slide the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;  /* a(n) after loop */
}
