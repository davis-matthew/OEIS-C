/*
 * A350920  a(0) = 5, a(1) = 5, and a(n) = 4*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5, a(1) = 5, a(n) = 4*a(n-1) - a(n-2) - 4  for n >= 2 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0 || n == 1)
        return (seq_t)5;

    /* initialise first two terms */
    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)5;   /* a(1) */
    seq_t curr  = 0;

    /* iterate up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        curr = ((seq_t)4) * prev1 - prev2 - (seq_t)4;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
