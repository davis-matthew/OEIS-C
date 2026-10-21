/*
 * A269701  Cyclic Fibonacci sequence, restricted to maximum=6.
 * https://oeis.org/A269701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0) = 0, a(1) = 1 a(n) = a(n-1) + a(n-2) if a(n-1) + a(n-2) ≤ 6
 *            a(n) = a(n-1) + a(n-2) - 6 if a(n-1) + a(n-2) > 6 for all n ≥ 2
 *            (This implements the cyclic Fibonacci sequence with maximum 6.)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;        /* a(n-2)  */
    seq_t prev1 = (seq_t)1;        /* a(n-1)  */
    seq_t cur   = 0;

    /* Iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2;
        if (cur > (seq_t)6) {
            cur -= (seq_t)6;
        }
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
