/*
 * A294255  E.g.f.: exp((1-x)*(1-x^2) - 1).
 * https://oeis.org/A294255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294255
 * E.g.f.: exp((1-x)*(1-x^2) - 1)  = exp(-x - x^2 + x^3)
 * Recurrence:
 *   a(0) = 1,  a(1) = a(2) = -1
 *   a(n) = -a(n-1) - 2*(n-1)*a(n-2) + 3*(n-1)*(n-2)*a(n-3)   for n ≥ 3
 * This follows from the derivative relation E'(x)=f'(x)E(x)
 * with f(x) = -x - x^2 + x^3.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)-1;

    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)-1;  /* a(1) */
    seq_t prev1 = (seq_t)-1;  /* a(2) */
    seq_t curr = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = -prev1;
        curr -= ( (seq_t)2 * (seq_t)(i-1) * prev2 );
        curr += ( (seq_t)3 * (seq_t)(i-1) * (seq_t)(i-2) * prev3 );
        /* shift for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
