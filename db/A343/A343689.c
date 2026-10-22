/*
 * A343689  a(1)=0, a(2)=1, a(n) = (4*n-2)*a(n-1) + a(n-2), n > 2.
 * https://oeis.org/A343689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(n) = (4*n-2)*a(n-1) + a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Iteratively compute the recurrence using two previous values */
    seq_t prev2 = 0;  /* a(1) */
    seq_t prev1 = 1;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* coefficient (4*i-2) fits in idx_t for the intended ranges */
        seq_t coef = (seq_t)(4 * i - 2);
        seq_t cur   = coef * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
