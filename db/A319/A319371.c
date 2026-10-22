/*
 * A319371  Numbers k such that the characteristic polynomial of a wheel graph
 *          of k nodes has exactly one monomial with vanishing coefficient.
 * https://oeis.org/A319371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n-2)/3) (for n ≥ 1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 the subtraction would underflow in unsigned arithmetic
       so handle it separately.  For all other n the formula works
       because n‑2 is non‑negative. */
    if (n == 1)
        return (seq_t)1;
    /* The division is performed in idx_t (unsigned), then cast to
       seq_t.  The addition is done in seq_t, which is large enough
       for all required values. */
    return (seq_t)n + (seq_t)((n - 2) / 3);
}
