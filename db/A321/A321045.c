/*
 * A321045  a(n) is the value of the first entry in the matrix A^n where A =
 *          [{1,2,3}, {4,5,6}, {7,8,9}].
 * https://oeis.org/A321045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1, a(1) = 1, a(2) = 30,
   a(n) = 15 * a(n‑1) + 18 * a(n‑2)   for n >= 3
   This recurrence follows from the characteristic polynomial
   λ³ – 15λ² – 18λ = 0 of the matrix A. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)30;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)30;  /* a(2) */
    seq_t a_cur   = a_prev1;    /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_cur = 15 * a_prev1 + 18 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
