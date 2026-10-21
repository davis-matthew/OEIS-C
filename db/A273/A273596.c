/*
 * A273596  For n >= 2, a(n) is the number of slim rectangular diagrams of
 *          length n.
 * https://oeis.org/A273596
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*a(n-1) + a(n-2) + 2, with a(2)=1, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the recurrence iteratively in O(n) time
   and uses no dynamic allocation, so it works for all provided seq_t and
   idx_t types. */

seq_t a(idx_t n)
{
    /* For indices below the sequence offset (n < 2), return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Base cases */
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;

    /* Iterative computation using two previous values only. */
    seq_t prev2 = (seq_t)1;   /* a(2) */
    seq_t prev1 = (seq_t)3;   /* a(3) */
    seq_t current;

    for (idx_t i = 4; i <= n; ++i) {
        current = ((seq_t)(i - 2)) * prev1 + prev2 + (seq_t)2;
        prev2   = prev1;
        prev1   = current;
    }

    return prev1;   /* a(n) */
}
