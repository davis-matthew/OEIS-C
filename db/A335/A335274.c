/*
 * A335274  a(n) = 2*a(n-1) + a(n-3), where a(0) = 0, a(1) = 1, a(2) = 4.
 * https://oeis.org/A335274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) + a(n‑3), a(0) = 0, a(1) = 1, a(2) = 4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Keep the last three values of the recurrence in a sliding window. */
    seq_t a0 = (seq_t)0; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)4; /* a(n-1) */
    seq_t ai;            /* a(n) */

    /* Compute up to the requested index. */
    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 * ((seq_t)2) + a0; /* a(n) = 2*a(n-1) + a(n-3) */
        a0 = a1;  /* shift window: a(n-3) <- a(n-2) */
        a1 = a2;  /* a(n-2) <- a(n-1) */
        a2 = ai;  /* a(n-1) <- a(n)   */
    }

    return a2;
}
