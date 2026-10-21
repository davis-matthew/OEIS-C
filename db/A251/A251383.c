/*
 * A251383  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2 3 4 5 or 6.
 * https://oeis.org/A251383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 6*a(n-3) Initial values: a(1) = 71,
 *            a(2) = 571, a(3) = 4585 (OEIS sequence A251383) This
 *            implementation computes the recurrence iteratively, requiring
 *            only O(1) extra space and O(n) time per call
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset.  Calls with n < 1 are not expected,
       but we return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    /* Return the initial terms directly. */
    if (n == 1)
        return (seq_t)71;
    if (n == 2)
        return (seq_t)571;
    if (n == 3)
        return (seq_t)4585;

    /* Iteratively compute the recurrence. */
    seq_t a1 = (seq_t)71;   /* a(1)   */
    seq_t a2 = (seq_t)571;  /* a(2)   */
    seq_t a3 = (seq_t)4585; /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)8 * a3 + a2 - (seq_t)6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
