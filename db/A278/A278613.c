/*
 * A278613  Sum of terms in level n of TRIP - Stern sequence associated with
 *          permutation triple (e,e,132).
 * https://oeis.org/A278613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1)+2*a(n-2)+6*a(n-3) with a(0)=3, a(1)=8, a(2)=21
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0u) return (seq_t)3;
    if (n == 1u) return (seq_t)8;
    if (n == 2u) return (seq_t)21;

    /* Variables to hold the last three terms of the recurrence. */
    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t curr  = (seq_t)21;  /* a(2) */

    /* Iteratively compute up to a(n). */
    for (idx_t i = 3u; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 2*a(i-2) + 6*a(i-3) */
        seq_t next = curr + (prev1 << 1) + (prev2 * (seq_t)6);
        prev2 = prev1;
        prev1 = curr;
        curr  = next;
    }
    return curr;
}
