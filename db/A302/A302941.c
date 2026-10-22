/*
 * A302941  Number of total dominating sets in the 2n-crossed prism graph.
 * https://oeis.org/A302941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 10*a(n-2) - a(n-3), with * a(1) = 9, a(2) =
 *            121, a(3) = 1296. (OEIS A302941) * The function recomputes the
 *            recurrence independently for each call, * requiring O(n) time
 *            and O(1) extra space
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset: for n < 1 we return 0 (undefined). */
    if (n == 0) {
        return (seq_t)0;
    }
    /* Base values for n = 1, 2, 3. */
    const seq_t a1 = (seq_t)9;   /* a(1) */
    const seq_t a2 = (seq_t)121; /* a(2) */
    const seq_t a3 = (seq_t)1296;/* a(3) */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Iteratively compute a(n) using the recurrence. */
    seq_t prev3 = a1;   /* a(i-3)  */
    seq_t prev2 = a2;   /* a(i-2)  */
    seq_t prev1 = a3;   /* a(i-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 10*a(i-1) + 10*a(i-2) - a(i-3) */
        next = (seq_t)10 * prev1 + (seq_t)10 * prev2 - prev3;
        /* Shift the window: a(i-3) = a(i-2), a(i-2) = a(i-1), a(i-1) = a(i) */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1; /* prev1 now holds a(n) */
}
