/*
 * A346397  Expansion of e.g.f. -log(1 - x) * exp(-2*x).
 * https://oeis.org/A346397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-3) * a(n-1) + 2 * (n-1) * a(n-2) + (-2)^(n-1) Initial
 *            values: a(0) = 0, a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev = (seq_t)0;          /* a(0) */
    seq_t a_curr = (seq_t)1;          /* a(1) */
    seq_t powNeg2 = (seq_t)(-2);      /* (-2)^(1) for n = 2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = ((seq_t)i - 3) * a_curr;                 /* (i-3)*a(n-1) */
        seq_t term2 = (seq_t)2 * (((seq_t)i) - 1) * a_prev;    /* 2*(i-1)*a(n-2) */
        seq_t next   = term1 + term2 + powNeg2;                /* recurrence */

        a_prev = a_curr;
        a_curr = next;

        powNeg2 = powNeg2 * (seq_t)(-2);                       /* prepare (-2)^(i) */
    }

    return a_curr;
}
