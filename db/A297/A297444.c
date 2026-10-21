/*
 * A297444  a(n) = a(n-1) + 9*a(n-2) - 9*a(n-3), where a(0) = 1, a(1) = 3,
 *          a(2) = 6, a(3) = 33.
 * https://oeis.org/A297444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+9*a(n-2)-9*a(n-3); a(0) = 1, a(1) = 3, a(2) = 6,
 *            a(3) = 33
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms without looping */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;
    if (n == 2u) return (seq_t)6;
    /* a(3) is needed as the starting point of the recurrence */
    const seq_t a3 = (seq_t)33;
    if (n == 3u) return a3;

    /* Keep the last three computed values:
       a_{k-3}, a_{k-2}, a_{k-1} respectively. */
    seq_t a_k_minus_3 = (seq_t)3;   /* a(1) */
    seq_t a_k_minus_2 = (seq_t)6;   /* a(2) */
    seq_t a_k_minus_1 = a3;         /* a(3) */
    seq_t a_k = 0;                  /* a(k) for k>=4 */

    /* Iterate from k = 4 up to n (inclusive) */
    for (idx_t k = 4; k <= n; ++k) {
        a_k = a_k_minus_1 + (seq_t)9 * a_k_minus_2 - (seq_t)9 * a_k_minus_3;
        /* Shift the window for the next iteration */
        a_k_minus_3 = a_k_minus_2;
        a_k_minus_2 = a_k_minus_1;
        a_k_minus_1 = a_k;
    }
    return a_k;
}
