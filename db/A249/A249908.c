/*
 * A249908  G.f. (1-x)/(2*sqrt(5*x^2 + 2*x + 1)) - 1/2.
 * https://oeis.org/A249908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 25, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) + (n+1)*a(n-1) + 3*n*a(n-2) + 5*(-n+3)*a(n-3) = 0
   Initial values: a(1) = -1, a(2) = 0, a(3) = 3
   This implementation computes a(n) in O(n) time without memoisation. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Handle the first three values directly. */
    if (n == 1)
        return (seq_t)(-1);
    if (n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)3;

    /* Variables hold a(i-3), a(i-2), a(i-1) respectively. */
    seq_t a1 = (seq_t)(-1);   /* a(1) */
    seq_t a2 = (seq_t)0;     /* a(2) */
    seq_t a3 = (seq_t)3;     /* a(3) */
    seq_t ai = a3;           /* placeholder for a(i) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* term1 = (i+1) * a(i-1) */
        seq_t term1 = (seq_t)(i + 1) * a3;

        /* term2 = 3 * i * a(i-2) */
        seq_t term2 = (seq_t)3 * (seq_t)i * a2;

        /* term3 = 5 * (3 - i) * a(i-3) */
        seq_t coeff = (seq_t)3 - (seq_t)i;       /* 3 - i (can be negative) */
        seq_t term3 = (seq_t)5 * coeff * a1;

        /* numerator = - (term1 + term2 + term3) */
        seq_t numerator = -(term1 + term2 + term3);

        /* a(i) = numerator / i */
        ai = numerator / (seq_t)i;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return ai;
}
