/*
 * A267528  Number of ON (black) cells in the n-th iteration of the "Rule 141"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) for n > 4 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5 The computation uses a
 *            simple O(n) recurrence without any dynamic allocation or caching
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the five initial terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;

    /* For n >= 5, compute iteratively using the recurrence. */
    seq_t v2 = 2;   /* a(2)   */
    seq_t v3 = 3;   /* a(3)   */
    seq_t v4 = 5;   /* a(4)   */
    seq_t res = 0;

    for (idx_t i = 5; i <= n; ++i) {
        res = v4 + v3 - v2;  /* a(i) = a(i-1)+a(i-2)-a(i-3) */
        v2 = v3;             /* shift the window */
        v3 = v4;
        v4 = res;
    }
    return res;
}
