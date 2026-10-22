/*
 * A369722  Expansion of e.g.f. exp( (3/2) * (1-sqrt(1-4*x)) ).
 * https://oeis.org/A369722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n-3)*a(n-1) + 9*a(n-2), with initial values a(0) =
 *            1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)3;

    /* Iterative computation of the linear recurrence.
       We keep only the last two terms to avoid large O(n) memory usage. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)3;   /* a(1) */
    seq_t a_curr;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* coeff = 2 * (2*i - 3) */
        seq_t coeff = (seq_t)2 * ( ((seq_t)2 * i) - (seq_t)3 );
        /* recurrence: a(i) = coeff * a_prev1 + 9 * a_prev2 */
        a_curr = coeff * a_prev1 + (seq_t)9 * a_prev2;
        /* shift the window */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;   /* a_curr holds a(n) after the loop */
}
