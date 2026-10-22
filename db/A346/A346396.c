/*
 * A346396  Expansion of e.g.f. -log(1 - x) * exp(4*x).
 * https://oeis.org/A346396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*a(n-1) - 4*(n-1)*a(n-2) + 4^(n-1) with a(0)=0,
 *            a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Recurrence variables */
    seq_t a_minus2 = (seq_t)0;   /* a(i-2)   */
    seq_t a_minus1 = (seq_t)1;   /* a(i-1)   */
    seq_t pow4     = (seq_t)4;   /* 4^(i-1)  for i=2 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = ((seq_t)i + (seq_t)3) * a_minus1;
        seq_t term2 = (seq_t)4 * ((seq_t)(i - 1)) * a_minus2;
        seq_t ai    = term1 - term2 + pow4;

        /* Shift variables for next iteration */
        a_minus2 = a_minus1;
        a_minus1 = ai;
        pow4 *= (seq_t)4; /* advance power of 4 for next i */
    }

    return a_minus1;
}
