/*
 * A288876  a(n) = binomial(n+4, n)^2. Square of the fifth diagonal sequence
 *          of A007318 (Pascal). Fifth diagonal sequence of A008459.
 * https://oeis.org/A288876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4, n)^2
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(n+4,4) = (n+4)(n+3)(n+2)(n+1)/24 */
    seq_t a1 = (seq_t)(n + 1);
    seq_t a2 = (seq_t)(n + 2);
    seq_t a3 = (seq_t)(n + 3);
    seq_t a4 = (seq_t)(n + 4);
    seq_t c = a1;
    c *= a2;
    c *= a3;
    c *= a4;
    c /= 24;                   /* exact division */
    return c * c;              /* square the binomial */
}
