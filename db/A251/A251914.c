/*
 * A251914  Numbers n such that the sum of the triangular numbers T(n) and
 *          T(n+1) is equal to the pentagonal number P(m) for some m.
 * https://oeis.org/A251914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3), with a(1)=0, a(2)=98,
 *            a(3)=9700. Offset: 1 (a(1) is the first listed term)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range indices (offset is 1). */
    if (n < 1) return 0;

    /* Handle the base cases directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)98;
    if (n == 3) return (seq_t)9700;

    /* Compute iteratively using the recurrence. */
    seq_t a_n_minus_3 = (seq_t)0;   /* a(n-3)   */
    seq_t a_n_minus_2 = (seq_t)98;  /* a(n-2)   */
    seq_t a_n_minus_1 = (seq_t)9700;/* a(n-1)   */
    seq_t a_n = 0;                  /* placeholder */

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n = (seq_t)99 * a_n_minus_1
              - (seq_t)99 * a_n_minus_2
              + a_n_minus_3;

        /* Shift the sliding window for the next iteration. */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}
