/*
 * A284351  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 899", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n even: a(n) = (10^(n+1)-1)/9; for n odd: a(n) =
 *            (10^(n+1)-91)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the decimal representation directly, avoiding overflow. */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;

    if ((n & 1U) == 0)          /* n even */
    {
        for (idx_t i = 0; i <= n; ++i)
            res = res * (seq_t)10 + (seq_t)1;
    }
    else                        /* n odd */
    {
        for (idx_t i = 0; i <= n; ++i)
        {
            if (i == n - 1)          /* second‑last digit is 0 */
                res = res * (seq_t)10 + (seq_t)0;
            else if (i == n)         /* last digit is 1 */
                res = res * (seq_t)10 + (seq_t)1;
            else                     /* all other digits are 1 */
                res = res * (seq_t)10 + (seq_t)1;
        }
    }

    return res;
}
