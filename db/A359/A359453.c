/*
 * A359453  Number of vertices in the partite set of the n-Menger sponge graph
 *          that do not contain the corners.
 * https://oeis.org/A359453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20^n - (-4)^n)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 12, a(n) = 16*a(n-1) + 80*a(n-2) for n >= 2 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)12;

    /* Compute the sequence iteratively using the two‑term recurrence. */
    seq_t prev2 = (seq_t)0;  /* a(n-2) */
    seq_t prev1 = (seq_t)12; /* a(n-1) */
    seq_t cur   = prev1;     /* will hold a(n) */

    /* Loop from 2 up to n inclusive. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* cur = 16*prev1 + 80*prev2.  Multipliers are small integers,
           so the intermediate products can be handled by seq_t itself,
           and no overflow occurs for all n whose a(n) fits in seq_t. */
        cur = prev1 * (seq_t)16 + prev2 * (seq_t)80;

        /* Shift the window for the next iteration. */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
