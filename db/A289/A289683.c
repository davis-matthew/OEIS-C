/*
 * A289683  Mixing moments of the busy period of mean steady-state 1/2 in an
 *          M/M/1 waiting process.
 * https://oeis.org/A289683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3; for n≥3, a(n) = (4*n-6)*a(n-1) -
 *            (n-1)*(n-3)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Previous two values: a(n-2) and a(n-1). */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)3;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* All intermediate arithmetic uses only the allowed operators
           on unsigned integer types (seq_t and idx_t). */
        seq_t factor1 = (seq_t)(4 * i - 6);
        seq_t factor2 = (seq_t)(i - 1) * (seq_t)(i - 3);

        cur = factor1 * prev1 - factor2 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
