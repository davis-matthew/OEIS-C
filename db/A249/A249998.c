/*
 * A249998  Expansion of 1/((1+x)*(1+3*x)*(1-4*x)).
 * https://oeis.org/A249998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n*3^(n+2)/14 + 4^(n+2)/35 - (-1)^n/10 Recurrence
 *            derived from the generating function: a(0)=1, a(1)=0, a(2)=13,
 *            a(n)=13*a(n-2)+12*a(n-3) for n≥3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)13;

    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)0;   /* a(1)   */
    seq_t a2 = (seq_t)13;  /* a(2)   */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = (seq_t)13 * a1 + (seq_t)12 * a0; /* a(i) = 13*a(i-2)+12*a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2; /* a(n) */
}
