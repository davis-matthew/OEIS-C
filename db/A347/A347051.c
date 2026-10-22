/*
 * A347051  a(0) = 1, a(1) = 2; a(n) = n * (n+1) * a(n-1) + a(n-2).
 * https://oeis.org/A347051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2
   a(n) = n*(n+1)*a(n-1) + a(n-2)      */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Iterate from 2 up to n computing the recurrence in O(n). */
    seq_t a_prev2 = (seq_t)1;          /* a(0) */
    seq_t a_prev  = (seq_t)2;          /* a(1) */
    seq_t a_cur   = 0;                 /* will hold the current term */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute n*(n+1) in seq_t to avoid overflow of idx_t. */
        seq_t factor = (seq_t)i * ((seq_t)i + (seq_t)1);
        a_cur = factor * a_prev + a_prev2;

        a_prev2 = a_prev;
        a_prev  = a_cur;
    }

    return a_cur;
}
