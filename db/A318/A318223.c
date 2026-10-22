/*
 * A318223  Expansion of e.g.f. exp(x/(1 + 2*x)).
 * https://oeis.org/A318223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1; for n≥1, a(n+1) = (1-4*n)*a(n) -
 *            4*n*(n-1)*a(n-1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t a_prev = 1;          /* a(n-1)  */
    seq_t a_curr = 1;          /* a(n)    */

    for (idx_t i = 1; i < n; ++i)
    {
        /* Compute a(i+1) using a_prev = a(i-1) and a_curr = a(i) */
        seq_t coeff = (seq_t)(1) - (seq_t)(4 * i);          /* 1 - 4*i */
        seq_t term1 = coeff * a_curr;                      /* (1-4*i)*a(i) */
        seq_t term2 = (seq_t)(4) * (seq_t)i * (seq_t)(i-1) * a_prev;
        seq_t a_next = term1 - term2;                      /* a(i+1) */

        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;   /* a(n) */
}
