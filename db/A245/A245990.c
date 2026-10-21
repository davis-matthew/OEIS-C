/*
 * A245990  Number of length n+2 0..3 arrays with no pair in any consecutive
 *          three terms totalling exactly 3.
 * https://oeis.org/A245990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) (offset 1) Initial values: a(1) = 28,
 *            a(2) = 68
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset – undefined, return 0 */
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)68;

    seq_t prev2 = (seq_t)28;   /* a(1)   */
    seq_t prev1 = (seq_t)68;   /* a(2)   */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 * (seq_t)2 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
